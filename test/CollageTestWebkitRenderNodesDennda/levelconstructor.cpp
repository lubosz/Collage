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


