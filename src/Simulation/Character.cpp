/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"
#include "Input.h"

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
  velocity.y -= 200.0 * d_t;
  if (moveConstraintMax.isZeroLength()) {
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_1)) {
      velocity.x = -40.0;
    }
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_2)) {
      velocity.x = 40.0;
    }
  } else {
    velocity = Ogre::Vector2::ZERO;
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT)) {
      velocity.x = 40.0 * -moveConstraintMin.y;
      velocity.y = 40.0 * moveConstraintMin.x;
    }
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT)) {
      velocity.x = 40.0 * moveConstraintMax.y;
      velocity.y = 40.0 * -moveConstraintMax.x;
    }
//    if (-moveConstraintMax.y < moveConstraintMax.x)
//      velocity = moveConstraintMax;
//    if (moveConstraintMin.y < -moveConstraintMin.x)
//      velocity = moveConstraintMin;
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP)
        && moveConstraintMin.y > 0.0
        && moveConstraintMax.y > 0.0) {
      velocity.y = 80.0;
    }
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_DOWN)
        && moveConstraintMin.y < 0.0
        && moveConstraintMax.y < 0.0) {
      velocity.y = -40.0;
    }
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
