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
  velocity = Ogre::Vector2(0.0, 0.0);
}

Character::~Character() {
}

void Character::init() {
  collisionShape.intitDebugRendering(sceneManager, id);
}

void Character::manipulate(float d_t) {
  if (d_t > 0.1) return;

  if (Animation::Instance().move) {
    // change speed when not in the air
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
  if (moveConstraintMin.y == 1.0 || moveConstraintMax.y == 1.0) {
    velocity.y = 0.0;
  }
  if (moveConstraintMin.y == -1.0 || moveConstraintMax.y == -1.0) {
    velocity.y = 0.0;
  }
  if (moveConstraintMin.x == 1.0 && velocity.x < 0.0) {
    velocity.x = 1.0;
    std::cout << "one" << std::endl;
  }
  if (moveConstraintMax.x == -1.0 && velocity.x > 0.0) {
    velocity.x = -1.0;
    std::cout << "two" << std::endl;
  }

  if (!Animation::Instance().move && !moveConstraintMax.isZeroLength()) {
    // character lands after jump
    Animation::Instance().move = true;
  }

  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP)
      && (moveConstraintMin.y == 1.0 || moveConstraintMax.y == 1.0)) {
    velocity.y = 200.0;
    Animation::Instance().move = false;
  }
  move(velocity.x * d_t, velocity.y * d_t);

//  std::cout << "moveConstraintMin: " << moveConstraintMin << std::endl;
//    // = 1/2*a*t^2+v*t
//////    test = false;
////    move(2.0 * moveConstraintMax.y * d_t, -2.0 * moveConstraintMax.x * d_t);
//    velocity = moveConstraintMax * velocity.length();
//  }

  collisionShape.updateDebugRendering(CollisionShape2::DEF_CONVEX);
}
