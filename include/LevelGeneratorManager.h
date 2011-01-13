/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVELGENERATORMANAGER_H
#define LEVELGENERATORMANAGER_H

#include <QObject>
#include <QUrl>
#include <QWebPage>
#include <QString>
#include <QtDebug>

#include <vector>

#include "LevelGenerator.h"
#include "GeneralLevelGenerator.h"

const float MAX_SCORE = 100.0;

class LevelGeneratorManager : public QObject {
    Q_OBJECT
 public:
    explicit LevelGeneratorManager(QObject *parent = 0);

    void addGenerator(LevelGenerator *generator);
    void requestWebpage(QString url);
    void sceneFromUrl(QString _url, Ogre::SceneManager * sceneManager);
    QWebPage webpage;
    Ogre::SceneManager * sceneManager;

 private:
    std::vector<LevelGenerator*> generators;
    bool requestLock;
		int percent;
    bool waitForSignal(QObject* obj, const char* signal, int timeout = 10000);

 signals:
    void levelGenerated(Level *level);

 private slots:
    void printProgress(int percent);
    void getMatchingGenerator(bool ok);
};

#endif
