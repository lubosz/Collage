#include "levelconstructor.h"

#include <iostream>
#include <QtWebKit>

LevelConstructor::LevelConstructor(): QObject(), m_percent(0)
{
    connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    connect(&m_page, SIGNAL(loadFinished(bool)), this, SLOT(doConstruction(bool)));
}

void LevelConstructor::construct(const QUrl &url)
{
    std::cout << "Loading " << qPrintable(url.toString()) << std::endl;
    m_percent = 0;
    //int index = outputFileName.lastIndexOf('.');
    //m_fileName = (index == -1) ? outputFileName + ".png" : outputFileName;
    m_page.mainFrame()->load(url);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_page.setViewportSize(QSize(1024, 768));
}

void LevelConstructor::printProgress(int percent)
{
    if (m_percent >= percent)
        return;

    while (m_percent++ < percent)
        std::cout << "#" << std::flush;
}

void LevelConstructor::saveResult(bool ok)
{
    std::cout << std::endl;
    qDebug() << "bla";

    // crude error-checking
    if (!ok) {
        std::cerr << "Failed loading " << qPrintable(m_page.mainFrame()->url().toString()) << std::endl;
        emit finished();
        return;
    }

    // save each frame in different image files
    doConstruction(m_page.mainFrame());

    emit finished();
}

inline int getColorimetricDistance(const QRgb &x, const QRgb &y) {
    // Returns squared distance between two colors.
    // Actually this shouldn't use RGB colorspace (for a proper 'distance')
    return pow(qRed(x) - qRed(y), 2.) +
           pow(qGreen(x) - qGreen(y), 2.) +
           pow(qBlue(x) - qBlue(y), 2.);
}

std::map<QRgb, int>
findDominantColors(const QImage &image, int max_dist, int min_cov)
{
    QRgb rgb, old;
    std::map<QRgb, int> colors, result;
    std::map<QRgb, int>::iterator it;
    int oldcount;
    double total = image.width() * image.height();
    bool merge;
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            merge = false;
            rgb = image.pixel(i, j);
            for (it = colors.begin(); it != colors.end(); it++) {
                old = (*it).first;
                oldcount = (*it).second;
                if (getColorimetricDistance(rgb, old) < pow(max_dist, 2.)) {
                    // This color is similar enough to a color we already
                    // have. Merge them afterwards.

                    // It's ok, let's just break here. Don't find closest.
                    merge = true;
                    break;
                }
            }
            if (merge) {
                QRgb first = qRgb((qRed(rgb) + qRed(old)) / 2,
                                  (qGreen(rgb) + qGreen(old)) / 2,
                                  (qBlue(rgb) + qBlue(old)) / 2);
                int second = oldcount++;
                colors.erase(it);
                colors.insert(std::pair<QRgb, int>(first, second));
            }
            else {
                colors.insert(std::pair<QRgb, int>(rgb, 1));
            }
        }
    }
    double cov = 0.;
    for (it = colors.begin(); it != colors.end(); it++) {
        cov = (*it).second / total;
        qDebug() << qRed((*it).first) << qGreen((*it).first) << qBlue((*it).first) << "covers" << cov * 100 << "%";
        if (min_cov < cov) {
            result.insert(std::pair<QRgb, int>((*it).first, cov));
        }
    }
    return result;
}

QRgb findDominantColor(const QImage &image) {
    QRgb rgb;
    int r = 0, g = 0, b = 0, total = image.width() * image.height();
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            rgb = image.pixel(i, j);
            r += qRed(rgb);
            g += qGreen(rgb);
            b += qBlue(rgb);
        }
    }
    qDebug() << "color: " << r/total << " " << g/total << " " << b/total;
    return qRgb(r/total, g/total, b/total);
}

void LevelConstructor::doConstruction(QWebFrame *frame)
{
    qint64 start = QDateTime::currentMSecsSinceEpoch();
    static int frameCounter = 0;

    QWebElement document = frame->documentElement();
    QWebElementCollection paragraphs = document.findAll("body");
    qDebug() << paragraphs.count();

    foreach(QWebElement p, paragraphs) {
        qDebug() << p.toPlainText();
        qDebug() << p.geometry();
        QString fileName(m_fileName);
        if (frameCounter) {
            int index = m_fileName.lastIndexOf('.');
            fileName = fileName.insert(index, "_frame" + QString::number(frameCounter));
        }

        QImage image(p.geometry().size(), QImage::Format_ARGB32_Premultiplied);
        qDebug() << p.geometry().size();
        image.fill(Qt::transparent);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.render(&painter);
        painter.end();

        image.save(fileName);
        findDominantColor(image);
        ++frameCounter;
        break;
    }
    qint64 end = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "Painting took " << (end - start) / 1000 << "secs";
}


