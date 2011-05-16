/*
 *  Copyright 2010 The Collage Project
 */
#include "FrameCapture.h"

#include <QtWebKit>
#include <QString>
#include <vector>
#include <QSignalSpy>
#include "System.h"
#include <QtCore>

FrameCapture::FrameCapture()
:
  QObject(), m_percent(0) {
  connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
  connect(&manager, SIGNAL(finished(QNetworkReply*)), this,
      SLOT(downloadFinished(QNetworkReply*)));
}

/**
 * Starts an event loop that runs until the given signal is received.
 * Optionally the event loop
 * can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
bool FrameCapture::waitForSignal(QObject* obj, const char* signal, int timeout =
    10000) {
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

QNetworkReply * FrameCapture::download(const QUrl &url) {
  return manager.get(QNetworkRequest(url));
}

bool FrameCapture::saveToDisk(const QString &filename, QIODevice *data) {
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    fprintf(stderr, "Could not open %s for writing: %s\n",
        qPrintable(filename), qPrintable(file.errorString()));
    return false;
  }

  file.write(data->readAll());
  file.close();

  return true;
}

void FrameCapture::downloadFinished(QNetworkReply *reply) {
  QUrl url = reply->url();
  QString extension = url.toString().right(4);
  QString fileName = "../Media/Textures/wall" + extension;

  if (reply->error()) {
    fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(),
        qPrintable(reply->errorString()));
  } else {
    if (saveToDisk(fileName, reply))
      printf("Download of %s succeeded (saved to %s)\n",
          url.toEncoded().constData(), qPrintable(fileName));
  }

  reply->deleteLater();
  emit finished();
}

void FrameCapture::loadUrl(const QUrl &url) {
  printf("\nLoading %s\n", qPrintable(url.toString()));
  m_percent = 0;
  m_page.mainFrame()->load(url);
}

void FrameCapture::saveWallPaper(
    const QUrl &url, const QString &outputFileName) {
  int timeout = 20000;
  m_fileName = outputFileName;

  // disable all but html
  m_page.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
  m_page.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
  m_page.settings()->setAttribute(QWebSettings::JavaEnabled, false);
  m_page.settings()->setAttribute(QWebSettings::PluginsEnabled, false);
  m_page.settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  m_page.settings()->setAttribute(
      QWebSettings::JavascriptCanOpenWindows, false);

  loadUrl(url);

  waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

  loadUrl(getFirstAttribute("a", "href", "http://wallbase.cc/wallpaper/"));

  waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

  QString imageUrl = getFirstAttribute("img", "src", "wallpaper-");

  printf("\nImage Url %s\n", qPrintable(imageUrl));

  if (imageUrl.contains(".jpg"))
    download(imageUrl);
  else
    return;

  // wait for wallpaper to download
  waitForSignal(this, SIGNAL(finished()), timeout);
}

void FrameCapture::urlToOgreImage(
    const QUrl &url, const QString &targetImage) {
  this->targetImage = targetImage;
  int timeout = 20000;

#ifdef FREEIMAGE_BUG
  m_page.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
#endif
  loadUrl(url);
  waitForSignal(m_page.mainFrame(), SIGNAL(loadFinished(bool)), timeout);

  std::cout << "\n";

  System::Instance().logMessage("Setup rendering");
  saveResult(true);
}

void FrameCapture::printProgress(int percent) {
  if (m_percent >= percent)
    return;

  while (m_percent++ < percent)
    std::cout << "#" << std::flush;
}

void FrameCapture::saveResult(bool ok) {
  std::cout << std::endl;

  // crude error-checking
  if (!ok) {
    std::cerr << "Failed loading "
        << qPrintable(m_page.mainFrame()->url().toString()) << std::endl;
    emit
    finished();
    return;
  }

  // save each frame in different image files
  saveFrame(QSize(1280, 1280));

  emit finished();
}

QString FrameCapture::getFirstAttribute(const QString & tag,
    const QString & attrib, const QString & match) {
  QWebElement document = m_page.mainFrame()->documentElement();
  QWebElementCollection elements = document.findAll(tag);
  if (elements.count() > 0) {
    foreach(QWebElement element, elements)
        if (element.attribute(attrib).contains(match)) {
          return element.attribute(attrib);
        }
    printf("\nNo match for %s.\n", qPrintable(match));
    foreach(QWebElement element, elements)
        printf("%s\n", qPrintable(element.attribute(attrib)));
  } else {
    printf("\nNo <%s> tags found in %s\n", qPrintable(tag),
        qPrintable(m_page.mainFrame()->baseUrl().toString()));
  }
}

void FrameCapture::saveFrame(const QSize & siteResolution) {
  m_page.mainFrame()->setScrollBarPolicy(
      Qt::Vertical, Qt::ScrollBarAlwaysOff);
  m_page.mainFrame()->setScrollBarPolicy(
      Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  m_page.setViewportSize(siteResolution);
  QImage image(siteResolution, QImage::Format_ARGB32_Premultiplied);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  m_page.mainFrame()->documentElement().render(&painter);
  painter.end();

  Ogre::TextureManager::getSingleton().remove(
      targetImage.toStdString());
  Ogre::TextureManager::getSingleton().loadImage(
      targetImage.toStdString(),
      "General",
      Ogre::Image().loadDynamicImage(
          image.bits(),
          image.width(),
          image.height(),
          1,
          Ogre::PF_A8R8G8B8));
}

