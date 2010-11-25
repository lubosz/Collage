#ifndef LEVELCONSTRUCTOR_H
#define LEVELCONSTRUCTOR_H

#include <QtWebKit>

class LevelConstructor : public QObject
{
    Q_OBJECT

public:
    LevelConstructor();
    void construct(const QUrl &url);

signals:
    void finished();

private slots:
    void printProgress(int percent);
    void saveResult(bool ok);

private:
    QWebPage m_page;
    QString m_fileName;
    int m_percent;

    void doConstruction(QWebFrame *frame);
};

#endif
