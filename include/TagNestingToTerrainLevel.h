/*
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "Level.h"

class TagNestingToTerrainLevel : public Level {
 public:
  explicit TagNestingToTerrainLevel(Ogre::SceneManager *sceneManager);

  float getScore(QWebPage *webpage);
  void generate();

  void addBox(unsigned height, unsigned count,
    Ogre::ManualObject *manual);
};

