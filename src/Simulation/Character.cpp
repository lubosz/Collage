/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"

Character::Character() {
}

Character::~Character() {
}

void Character::init() {
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Character::manipulate(float d_t) {
  move(0.5 * d_t, -5.0 * d_t * d_t + velocity.y * d_t);  // s = 1/2*a*t^2 + v*t
  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}
