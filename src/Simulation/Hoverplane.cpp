/*
 * Hoverplane.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Hoverplane.h"

Hoverplane::Hoverplane() {
  u = 0.0;
  speed = 1.0;
  incr = true;
}

Hoverplane::~Hoverplane() {
}

void Hoverplane::init() {
//  collisionShape.intitDebugRendering(sceneManager, id);
}

void Hoverplane::manipulate(float d_t) {
  if (d_t > 0.1) return;
  Ogre::Vector2 lastPos = p1 + u * (p2 - p1);
  if (incr) {
    u += d_t * speed;
    if (u > 1.0) {
      u = 1.0;
      incr = false;
    }
  } else {
    u -= d_t * speed;
    if (u < 0.0) {
      u = 0.0;
      incr = true;
    }
  }
  Ogre::Vector2 currentPos = p1 + u * (p2 - p1);
  move(currentPos - lastPos);

//  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}

Hoverplane* Hoverplane::setPath(float x1, float y1, float x2, float y2) {
  p1 = Ogre::Vector2(x1, y1);
  p2 = Ogre::Vector2(x2, y2);
  teleport(p1);
}

Hoverplane* Hoverplane::setSpeed(float speed) {
  this->speed = speed;
}
