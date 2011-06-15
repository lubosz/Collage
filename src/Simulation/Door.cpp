/*
 * Door.cpp
 *
 *  Created on: 24.01.2011
 *      Author: Deine Mutter
 *  Copyright 2010 The Collage Project
 */

#include "Door.h"

Door::Door() {
}

Door::~Door() {
}

void Door::init() {
  // collisionShape.intitDebugRendering(sceneManager, id);
}

void Door::manipulate(float d_t) {
  // XXX Why does this crash with DEF_AABB?
  // collisionShape.updateDebugRendering(CollisionShape2::DEF_CONVEX);
}
