/*
 *  Copyright 2010 The Collage Project
 */
#ifndef GENERALLEVEL_H
#define GENERALLEVEL_H

#include "Level.h"

class GeneralLevel : public Level {
    Q_OBJECT
 public:
    explicit GeneralLevel(QObject *parent = 0);

    float getScore(QWebPage *webpage);
    void generate(Ogre::SceneManager *sceneManager);

 signals:

  public slots:
};

#endif
