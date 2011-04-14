/*
 * Terrain.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Terrain.h"

Terrain::Terrain() {
}

Terrain::~Terrain() {
}

void Terrain::init() {
  std::vector<Ogre::Vector2> points;
  points.push_back(Ogre::Vector2(1.0, 1.0));
  points.push_back(Ogre::Vector2(0.0, 0.0));
  collisionShape.addPoints(points);
  collisionShape.intitDebugRendering(debugRendererSceneManager, id);
}

void Terrain::manipulate(float d_t) {
  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}
