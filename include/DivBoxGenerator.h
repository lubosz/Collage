/*
 *  Copyright 2010 The Collage Project
 */
#pragma once

#include "LevelGenerator.h"

class DivBoxGenerator : public LevelGenerator {
 public:
    DivBoxGenerator();

    float getScore(QWebPage *webpage);
    Level* generate(Ogre::SceneManager * sceneManager);
};
