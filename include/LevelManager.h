/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QObject>
#include <QUrl>
#include <QWebPage>
#include <QString>
#include <QtDebug>

#include <vector>

#include "Level.h"
#include "GeneralLevel.h"
#include "DivBoxLevel.h"
#include "TagNestingToTerrainLevel.h"

const float MAX_SCORE = 100.0;

class LevelManager : public QObject {
    Q_OBJECT
 public:
    explicit LevelManager(QObject *parent = 0);
    LevelManager(Ogre::SceneManager *scMgr);

    void addLevel(Level *level);
    void requestWebpage(QString url);
    void sceneFromUrl(QString _url, Ogre::SceneManager *sceneManager);
    QWebPage webpage;
    Ogre::SceneManager * sceneManager;

 private:
    std::vector<Level*> levels;
    bool requestLock;
    int percent;
    bool waitForSignal(QObject* obj, const char* signal, int timeout = 10000);

 signals:
    void levelGenerated(Level *level);

 private slots:
    void printProgress(int percent);
    void getMatchingLevel(bool ok);
};

#endif
