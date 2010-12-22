#ifndef LEVELGENERATORMANAGER_H
#define LEVELGENERATORMANAGER_H

#include <QObject>
#include <QUrl>
#include <QWebPage>
#include <QtDebug>

#include <vector>
#include <iostream>

#include "levelgenerator.h"



class LevelGeneratorManager : public QObject
{
    Q_OBJECT
public:
    explicit LevelGeneratorManager(QObject *parent = 0);

    void addGenerator(LevelGenerator *generator);
    void requestWebpage(QUrl url);

private:
    std::vector<LevelGenerator*> generators;
    QWebPage webpage;
    bool requestLock;
    int percent;

signals:
    void finished();

private slots:
    void printProgress(int percent);
    void getMatchingGenerator(bool ok);
};

#endif // LEVELGENERATORMANAGER_H
