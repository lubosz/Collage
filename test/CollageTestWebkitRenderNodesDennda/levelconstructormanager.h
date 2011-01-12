#ifndef LEVELCONSTRUCTOR_H
#define LEVELCONSTRUCTOR_H

#include <vector>
#include <QtWebKit>

class LevelConstructorManager : public QObject
{
    Q_OBJECT

public:
    LevelConstructorManager();
    void findConstructor(const QUrl &url);
    //std::vector<LevelConstructor> constructors;

signals:
    void finished();

private slots:
    void printProgress(int percent);

private:
    QWebPage m_page;
    QString m_fileName;
    int m_percent;

    void dispatch(QWebFrame *frame);
};

#endif
