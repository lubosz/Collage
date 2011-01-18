/*
 * IHCharacterTerrain.cpp
 *
 *  Created on: 18.01.2011
 *      Author: lochmann
 *  Copyright 2011 The Collage Project
 */

#include "IHCharacterTerrain.h"

IHCharacterTerrain::IHCharacterTerrain() {
}

IHCharacterTerrain::~IHCharacterTerrain() {
}

void IHCharacterTerrain::enter(Interaction* interaction) {
  Actor* character = interaction->getA();
  character->addMotionLock(Ogre::Vector2::NEGATIVE_UNIT_Y);
}

void IHCharacterTerrain::inside(Interaction* interaction, float d_t) {
}
