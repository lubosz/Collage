#include "FrameCapture.h"

#include <iostream>
#include <QtWebKit>
#include <QString>
#include <vector>
#include <QSignalSpy>
#include "System.h"
#include <QtCore>

FrameCapture::FrameCapture(): QObject(), m_percent(0)
{
    connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    connect(&manager, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    //connect(&m_page, SIGNAL(loadFinished(bool)), this, SLOT(saveResult(bool)));
    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(downloadFinished(QNetworkReply*)));
}

/**
 * Starts an event loop that runs until the given signal is received.
 * Optionally the event loop
 * can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
bool FrameCapture::waitForSignal(QObject* obj, const char* signal, int timeout = 10000)
{
    QEventLoop loop;
    QObject::connect(obj, signal, &loop, SLOT(quit()));
    QTimer timer;
    QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
    if (timeout > 0) {
        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.setSingleShot(true);
        timer.start(timeout);
    }
    loop.exec();
    return timeoutSpy.isEmpty();
}

QNetworkReply * FrameCapture::download(const QUrl &url)
{
    return manager.get(QNetworkRequest(url));
}

bool FrameCapture::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void FrameCapture::downloadFinished(QNetworkReply *reply)
{
	QString fileName = "../Media/Textures/wall.jpg";
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (saveToDisk(fileName, reply))
            printf("Download of %s succeeded (saved to %s)\n",
                   url.toEncoded().constData(), qPrintable(fileName));
    }

    reply->deleteLater();
    emit finished();
}

void FrameCapture::loadUrl(const QUrl &url){
	printf("\nLoading %s\n", qPrintable(url.toString()));
    m_percent = 0;
    m_page.mainFrame()->load(url);
}

void FrameCapture::saveWallPaper(const QUrl &url, const QString &outputFileName)
{
	int timeout = 20000;
    m_fileName = outputFileName;

    //disable all but html
    m_page.settings()->setAttribute(QWebSettings::AutoLoadImages,false);
    m_page.settings()->setAttribute(QWebSettings::JavascriptEnabled,false);
    m_page.settings()->setAttribute(QWebSettings::JavaEnabled,false);
    m_page.settings()->setAttribute(QWebSettings::PluginsEnabled,false);
    m_page.settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled,true);
    m_page.settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,false);

    loadUrl(url);

    waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

    loadUrl(getFirstAttribute("a","href","http://wallbase.net/wallpaper/"));

    waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

    QString imageUrl = getFirstAttribute("img", "src", "wallpaper-");

    printf ("Image Url %s\n" ,qPrintable(imageUrl));

    if (imageUrl.contains(".jpg"))
		download(imageUrl);

    //wait for wallpaper to download
    waitForSignal(this, SIGNAL(finished()), timeout);
}

void FrameCapture::saveWebRender(const QUrl &url, const QString &outputFileName){
	m_fileName = outputFileName;
	int timeout = 20000;

#ifdef OGRE_PLATFORM_LINUX
	//
//	m_page.settings()->setAttribute(QWebSettings::AutoLoadImages,false);
//	QList<QString> exts;
//	exts.append("jpg");
//	exts.append("jpeg");
//	exts.append("JPG");
//	exts.append("JPEG");
//	// ("jpg","JPG")
//	//exts.append();
//	foreach(QString ext, exts)
//		{
//			QString code = "$('[src*=" + ext + "]').remove()";
//			m_page.mainFrame()->evaluateJavaScript(code);
//		}
				QString code = "$('[src*=jpg]').remove()";
				m_page.mainFrame()->evaluateJavaScript(code);
#endif
	//	connect(m_page.mainFrame(), SIGNAL(loadFinished(bool)), this,SLOT(saveResult(bool)));
	loadUrl(url);
	waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

//	m_page.settings()->setAttribute(QWebSettings::AutoLoadImages, true);

//	loadUrl(getFirstAttribute("img", "src", "wallpaper-"));

	System::Instance().logMessage("Setup rendering");
	m_page.mainFrame()->setScrollBarPolicy(Qt::Vertical,
			Qt::ScrollBarAlwaysOff);
	m_page.mainFrame()->setScrollBarPolicy(Qt::Horizontal,
			Qt::ScrollBarAlwaysOff);
	m_page.setViewportSize(QSize(1280, 1280));
	saveResult(true);
//    saveFrame(m_page.mainFrame());

//    emit finished();
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

QString FrameCapture::getFirstAttribute(const QString &  tag, const QString &  attrib, const QString &  match){
    QWebElement document = m_page.mainFrame()->documentElement();
	QWebElementCollection elements = document.findAll(tag);
    if(elements.count() > 0){
		foreach (QWebElement element, elements)
			if (element.attribute(attrib).contains(match)){
				return element.attribute(attrib);
			}
		printf("\nNo match for %s.\n", qPrintable(match));
		foreach (QWebElement element, elements)
			printf("%s\n", qPrintable(element.attribute(attrib)));
		exit(0);
    }else{
    	printf("\nNo <%s> tags found in %s\n",
    			qPrintable(tag),qPrintable(m_page.mainFrame()->baseUrl().toString()));
    	exit(0);
    }

}

void FrameCapture::saveFrame(QWebFrame *frame)
{
    static int frameCounter = 0;

    QString fileName(m_fileName);

    QImage image(QSize(1280, 1280), QImage::Format_ARGB32_Premultiplied);
//    image.fill(Qt::transparent);
    if(image.width() != 1280 || image.height() != 1280){
    	System::Instance().logMessage("WRONG RESOLUTION!");
    	printf("width: %d, height %d", image.width(), image.height());
    	exit(0);
    }
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    frame->documentElement().render(&painter);
    painter.end();

    image.save(fileName);

}

