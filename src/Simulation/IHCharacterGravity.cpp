/*
 * IHCharacterGravity.cpp
 *
 *  Created on: 14.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "IHCharacterGravity.h"

IHCharacterGravity::IHCharacterGravity(Ogre::Vector2 gravity) {
  this->gravity = gravity;
}

IHCharacterGravity::~IHCharacterGravity() {
  // TODO(gerrit): Auto-generated destructor stub
}

void IHCharacterGravity::inside(Interaction* interaction, float d_t) {
  Actor* character = interaction->getA();
  character->addVelocity(to3D(gravity)*d_t);
}
