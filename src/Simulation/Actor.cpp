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

Ogre::SceneNode *Actor::getSceneNode() const {
    return sceneNode;
}

void Actor::update(float d_t) {
}

void Actor::print() {
  printf("empty");
}
