/*
 * LevelState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: deine mudda
 *  Copyright 2010 The Collage Project
 */

#include <iostream>
#include "LevelState.h"
#include "IHCharacterSteering.h"


IHCharacterSteering::IHCharacterSteering() {
}

void IHCharacterSteering::inside(Interaction* interaction, float d_t) {
  Ogre::Vector3 moveVec = Vector3::ZERO;
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
    moveVec += Ogre::Vector3(-1., 0., 0.);

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
    moveVec += Ogre::Vector3(1., 0., 0.);

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_SPACE))
    moveVec += Ogre::Vector3(0., 1., 0.);

  // if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
  //   moveVec += Ogre::Vector3(0., 1., 0.);
  interaction->getA()->addVelocity(moveVec);
}
