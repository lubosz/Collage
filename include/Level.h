/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>

#include "Simulation.h"

class Level : public QObject {
    Q_OBJECT
 public:
    Ogre::SceneManager* sceneManager;

    Level();

};

#endif
