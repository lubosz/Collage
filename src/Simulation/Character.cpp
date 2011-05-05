/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"

Character::Character() {
  test = true;
}

Character::~Character() {
}

void Character::init() {
  moveConstraintMin = Ogre::Vector2::ZERO;
  moveConstraintMax = Ogre::Vector2::ZERO;
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Character::manipulate(float d_t) {
  if (moveConstraintMin.isZeroLength()) {
    move(0.0 * d_t, -5.0 * d_t * d_t + velocity.y * d_t);  // s = 1/2*a*t^2+v*t
  } else {
//    test = false;
//    move(-moveConstraintMin.y * d_t, moveConstraintMin.x * d_t);
  }

  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}
