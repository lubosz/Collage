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
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Terrain::manipulate(float d_t) {
  move(velocity * d_t);
  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}
