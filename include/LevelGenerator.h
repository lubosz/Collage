/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QObject>
#include <QWebFrame>
#include "Level.h"

class LevelGenerator : public QObject {
    Q_OBJECT
 public:
    explicit LevelGenerator(QObject *parent = 0);

    virtual float getScore(QWebPage *webpage) = 0;
    virtual Level* generate(Ogre::SceneManager * sceneManager) = 0;

 protected:
    QWebPage *webpage;

 signals:

  public slots:
};

#endif
