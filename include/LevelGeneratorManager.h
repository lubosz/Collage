#ifndef LEVELGENERATORMANAGER_H
#define LEVELGENERATORMANAGER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QWebPage>
#include <QtDebug>

#include <vector>
#include <iostream>

#include "LevelGenerator.h"
#include "GeneralLevelGenerator.h"

const float MAX_SCORE = 100.0;

class LevelGeneratorManager : public QObject
{
    Q_OBJECT
public:
    explicit LevelGeneratorManager(QObject *parent = 0);

    void addGenerator(LevelGenerator *generator);
    void requestWebpage(QString url);

private:
    std::vector<LevelGenerator*> generators;
    QWebPage webpage;
    bool requestLock;
    int percent;

signals:
    void levelGenerated(Level *level);

private slots:
    void printProgress(int percent);
    void getMatchingGenerator(bool ok);
};

#endif // LEVELGENERATORMANAGER_H
