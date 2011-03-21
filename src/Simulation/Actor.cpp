/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Actor.h"
#include <QtCore>

Actor::Actor() {
  }

Actor::~Actor() {
}

Ogre::Vector2 Actor::getPosition() {
  return to2D(sceneNode->getPosition());
}

void Actor::setPosition(Ogre::Vector2 pos) {
  sceneNode->setPosition(to3D(pos, sceneNode->getPosition().z));
}

void Actor::update(float d_t) {
}

void Actor::print() {
  printf("empty");
}
