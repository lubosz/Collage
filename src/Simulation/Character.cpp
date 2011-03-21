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

void Character::print() {
  printf("character");
}

void Character::move(Ogre::Vector2 vec) {
  oldPosition = getPosition();
  sceneNode->setPosition(sceneNode->getPosition() + to3D(vec));
}
