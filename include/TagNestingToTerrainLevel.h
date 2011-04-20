/*
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "Level.h"

class TagNestingToTerrainLevel : public Level {
	Q_OBJECT
 public:
		explicit TagNestingToTerrainLevel(QObject *parent = 0);

		float getScore(QWebPage *webpage);
		void generate(Ogre::SceneManager *sceneManager);

    void addBox(unsigned height, unsigned count,
                Ogre::ManualObject *manual);
};

