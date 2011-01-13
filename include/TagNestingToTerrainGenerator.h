/*
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "LevelGenerator.h"

class TagNestingToTerrainGenerator : public LevelGenerator {
	Q_OBJECT
	public:
		explicit TagNestingToTerrainGenerator(QObject *parent = 0);

		float getScore(QWebPage *webpage);
		Level* generate();

    void addBox(unsigned height);

	signals:

  public slots:
};

