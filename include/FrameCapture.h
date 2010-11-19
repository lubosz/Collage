#pragma once

#include <QtWebKit>
#include <string>

using std::string;

class FrameCapture : public QObject
{
    Q_OBJECT

public:
    FrameCapture();
    void load(const QUrl &url, const QString &outputFileName);
    QString getFirstAttribute(const QString &  tag, const QString &  attrib, const QString &  match);
    bool waitForSignal(QObject* obj, const char* signal, int timeout);

signals:
    void finished();

private slots:
    void printProgress(int percent);
    void saveResult(bool ok);

private:
    QWebPage m_page;
    QString m_fileName;
    int m_percent;

    void saveFrame(QWebFrame *frame);
};
