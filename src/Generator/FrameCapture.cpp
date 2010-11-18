#include "FrameCapture.h"

#include <iostream>
#include <QtWebKit>
#include <QString>
#include <vector>

FrameCapture::FrameCapture(): QObject(), m_percent(0)
{
    connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    connect(&m_page, SIGNAL(loadFinished(bool)), this, SLOT(saveResult(bool)));
}

void FrameCapture::load(const QUrl &url, const QString &outputFileName)
{



    std::cout << "Loading " << qPrintable(url.toString()) << std::endl;
    m_percent = 0;
    int index = outputFileName.lastIndexOf('.');
    m_fileName = (index == -1) ? outputFileName + ".png" : outputFileName;
    m_page.mainFrame()->load(url);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_page.setViewportSize(QSize(1920, 1200));





}

void FrameCapture::printProgress(int percent)
{
    if (m_percent >= percent)
        return;

    while (m_percent++ < percent)
        std::cout << "#" << std::flush;
}

void FrameCapture::saveResult(bool ok)
{
    std::cout << std::endl;

    // crude error-checking
    if (!ok) {
        std::cerr << "Failed loading " << qPrintable(m_page.mainFrame()->url().toString()) << std::endl;
        emit finished();
        return;
    }

    // save each frame in different image files
    saveFrame(m_page.mainFrame());

    emit finished();
}

void FrameCapture::saveFrame(QWebFrame *frame)
{
    QWebElement document = m_page.mainFrame()->documentElement();

    /*
    //Find url
    QWebElementCollection elements = document.findAll("a");
    QString wallpaperUrl;
    foreach (QWebElement element, elements)
		if (element.attribute("href").contains("http://wallbase.net/wallpaper/")){
			wallpaperUrl = element.attribute("href");
			qDebug("HREF: %s", element.attribute("href").toAscii().data());
			break;
		}
     */

    //Find image
    QWebElementCollection elements = document.findAll("img");
    QString wallpaperUrl;
    foreach (QWebElement element, elements)
		if (element.attribute("src").contains("wallpaper-")){
			wallpaperUrl = element.attribute("src");
	    	qDebug("HREF: %s", element.attribute("src").toAscii().data());
			break;
		}

    static int frameCounter = 0;

    QString fileName(m_fileName);
    if (frameCounter) {
        int index = m_fileName.lastIndexOf('.');
        fileName = fileName.insert(index, "_frame" + QString::number(frameCounter));
    }

    QImage image(frame->contentsSize(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    frame->documentElement().render(&painter);
    painter.end();

    image.save(fileName);
/*
    ++frameCounter;
    foreach(QWebFrame *childFrame, frame->childFrames())
        saveFrame(childFrame);
	*/
}

