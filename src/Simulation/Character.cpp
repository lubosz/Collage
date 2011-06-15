/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"
#include "Input.h"
#include "Animation.h"

Character::Character() {
  test = true;
  jumpAbility = 0;
  jumpTime = 0.0;
  velocity = Ogre::Vector2(0.0, 0.0);
}

Character::~Character() {
}

void Character::init() {
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Character::manipulate(float d_t) {
  if (d_t > 0.1) return;

  if (Animation::Instance().isrunning) {
    // change speed only when not in the air
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) {
      Animation::Instance().moveSpeed = 4.0;
    } else {
      Animation::Instance().moveSpeed = 2.0;
    }
  }

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A)) {
    std::cout << moveConstraintMin << " " << moveConstraintMax << std::endl;
  }

  velocity.y -= 200.0 * d_t;
  velocity.x = 0.0;
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT)) {
    velocity.x = -60.0 * Animation::Instance().moveSpeed;
    Animation::Instance().activate();
    sceneManager->getSceneNode("Armature")->setOrientation(
        Ogre::Quaternion(Ogre::Degree(-90.0), Ogre::Vector3::UNIT_Y));
  }
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT)) {
    velocity.x = 60.0 * Animation::Instance().moveSpeed;
    Animation::Instance().activate();
    sceneManager->getSceneNode("Armature")->setOrientation(
        Ogre::Quaternion(Ogre::Degree(90.0), Ogre::Vector3::UNIT_Y));
  }

  if (!Animation::Instance().isrunning && !moveConstraintMax.isZeroLength()) {
    // character lands after jump
    Animation::Instance().isrunning = true;
  }

  if (moveConstraintMin.y == 1.0 || moveConstraintMax.y == 1.0) {
    if (jumpAbility > 1) {
      jumpAbility = 0;
    }
    jumpTime = 0.0;
  } else {
    jumpTime += d_t;
    Animation::Instance().isrunning = false;
  }
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP)) {
    if ((jumpAbility == 1) || (jumpAbility == 2 && jumpTime > 0.2)) {
      velocity.y = 100.0;
      jumpAbility++;
      Animation::Instance().isrunning = false;
    }
  } else {
    if (jumpAbility == 0) {
      jumpAbility = 1;
    }
  }

//  velocity = Ogre::Vector2::ZERO;
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
//    velocity.y = 80;
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_DOWN))
//    velocity.y = -80;
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT))
//    velocity.x = -80;
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
//    velocity.x = 80;

  if (velocity.x < 0.0 &&
      (moveConstraintMin.x == 1.0 || moveConstraintMax.x == 1.0))
    velocity.x = 0.0;
  if (velocity.x > 0.0 &&
      (moveConstraintMin.x == -1.0 || moveConstraintMax.x == -1.0))
    velocity.x = 0.0;
  if (velocity.y < 0.0 &&
      (moveConstraintMin.y == 1.0 || moveConstraintMax.y == 1.0))
    velocity.y = 0.0;
  if (velocity.y > 0.0 &&
      (moveConstraintMin.y == -1.0 || moveConstraintMax.y == -1.0))
    velocity.y = 0.0;

  move(velocity.x * d_t, velocity.y * d_t);

//  std::cout << "moveConstraintMin: " << moveConstraintMin << std::endl;
//    // = 1/2*a*t^2+v*t
//////    test = false;
////    move(2.0 * moveConstraintMax.y * d_t, -2.0 * moveConstraintMax.x * d_t);
//    velocity = moveConstraintMax * velocity.length();
//  }

  collisionShape.updateDebugRendering(CollisionShape2::DEF_CONVEX);
}
