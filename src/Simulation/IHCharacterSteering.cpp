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
  float speed = 10.;
  Ogre::Vector3 moveVec = Vector3::ZERO;
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    moveVec += Ogre::Vector3(-1., 0., 0.);

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    moveVec += Ogre::Vector3(1., 0., 0.);

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
    moveVec += Ogre::Vector3(0., 1., 0.);

  // if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
  //   moveVec += Ogre::Vector3(0., 1., 0.);
  interaction->getA()->addTranslation(moveVec * d_t * speed);
}
