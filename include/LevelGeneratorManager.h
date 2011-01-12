#ifndef LEVELGENERATORMANAGER_H
#define LEVELGENERATORMANAGER_H

#include <QObject>
#include <QUrl>
#include <QWebPage>
#include <QString>
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
    QWebPage webpage;

private:
    std::vector<LevelGenerator*> generators;
    bool requestLock;
    int percent;

signals:
    void levelGenerated(Level *level);

public slots:
    void printProgress(int percent);
    void getMatchingGenerator(bool ok);
};

#endif // LEVELGENERATORMANAGER_H
