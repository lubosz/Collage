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
  std::vector<Ogre::Vector2> points;
  points.push_back(Ogre::Vector2(-1.1, -1.1));
  points.push_back(Ogre::Vector2(-1.1, 1.1));
  points.push_back(Ogre::Vector2(1.1, 1.1));
  points.push_back(Ogre::Vector2(1.1, -1.1));
  points.push_back(Ogre::Vector2(1.0, -1.0));
  points.push_back(Ogre::Vector2(-2.0, 0.0));
  points.push_back(Ogre::Vector2(0.0, 1.2));
  collisionShape.addPoints(points);
  collisionShape.intitDebugRendering(debugRendererSceneManager, id);
}

void Character::manipulate(float d_t) {
  move(.5*d_t, -5. * d_t * d_t + this->moveVector.y);  // s=1/2at^2
  collisionShape.updateDebugRendering(CollisionShape2::DEF_AABB);
}
