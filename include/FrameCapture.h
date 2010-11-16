#pragma once

#include <Qt/QtWebKit>

class FrameCapture : public QObject
{
    Q_OBJECT

public:
    FrameCapture();
    void load(const QUrl &url, const QString &outputFileName);

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
