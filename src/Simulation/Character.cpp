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
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Character::manipulate(float d_t) {
//  std::cout << "moveConstraintMin: " << moveConstraintMin << std::endl;
  if (test && moveConstraintMax.isZeroLength()) {
    move(1.0 * d_t, -5.0 * d_t * d_t + velocity.y * d_t);  // = 1/2*a*t^2+v*t
  } else {
//    test = false;
    move(2.0 * moveConstraintMax.y * d_t, -2.0 * moveConstraintMax.x * d_t);
  }

  collisionShape.updateDebugRendering(CollisionShape2::DEF_CONVEX);
}
