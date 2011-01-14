/*
 *  Copyright 2010 The Collage Project
 */
#ifndef GENERALLEVELGENERATOR_H
#define GENERALLEVELGENERATOR_H

#include "LevelGenerator.h"

class GeneralLevelGenerator : public LevelGenerator {
    Q_OBJECT
 public:
    explicit GeneralLevelGenerator(QObject *parent = 0);

    float getScore(QWebPage *webpage);
    Level* generate(Ogre::SceneManager * sceneManager);

 signals:

  public slots:
};

#endif
