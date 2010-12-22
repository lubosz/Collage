#ifndef LEVELGENERATORMANAGER_H
#define LEVELGENERATORMANAGER_H

#include <QObject>
#include <QUrl>

#include <vector>

#include "levelgenerator.h"



class LevelGeneratorManager : public QObject
{
    Q_OBJECT
public:
    explicit LevelGeneratorManager(QObject *parent = 0);

    void addGenerator(LevelGenerator *generator);
    LevelGenerator* getMatchingGenerator(QUrl url);

private:
    std::vector<LevelGenerator*> generators;

signals:

public slots:

};

#endif // LEVELGENERATORMANAGER_H
