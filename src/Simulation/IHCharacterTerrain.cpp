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
}

void IHCharacterTerrain::inside(Interaction* interaction, float d_t) {
  Actor* character = interaction->getA();
  character->addMotionLock(Ogre::Vector2::UNIT_Y);

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
    character->addVelocity(Ogre::Vector3(0., 40.*d_t, 0.));
}

void IHCharacterTerrain::leave(Interaction* interaction) {
  Actor* character = interaction->getA();
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
    character->addVelocity(Ogre::Vector3(0., 30., 0.));
}
