#pragma once

#include <QtWebKit>
#include <string>

using std::string;

class FrameCapture : public QObject
{
    Q_OBJECT

public:
    FrameCapture();
    void saveWallPaper(const QUrl &url, const QString &outputFileName);
    void saveWebRender(const QUrl &url, const QString &outputFileName);
    QString getFirstAttribute(const QString &  tag, const QString &  attrib, const QString &  match);
    bool waitForSignal(QObject* obj, const char* signal, int timeout);
    QNetworkReply * download(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

signals:
    void finished();

private slots:
    void printProgress(int percent);
    void saveResult(bool ok);
    void downloadFinished(QNetworkReply *reply);

private:
    QWebPage m_page;
    QString m_fileName;
    int m_percent;
    QNetworkAccessManager manager;

    void saveFrame(QWebFrame *frame);
    void loadUrl(const QUrl &url);
};
