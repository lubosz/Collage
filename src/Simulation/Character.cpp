/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"
#include "input.h"

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
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A)) {
    std::cout << moveConstraintMin << " " << moveConstraintMax << std::endl;
  }
  velocity.y -= 200.0 * d_t;
  velocity.x = 0.0;
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT)) {
    velocity.x = -40.0;
  }
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT)) {
    velocity.x = 40.0;
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
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP)
      && (moveConstraintMin.y == 1.0 || moveConstraintMax.y == 1.0)) {
    velocity.y = 80.0;
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
