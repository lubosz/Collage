/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Actor.h"
#include <QtCore>

Actor::Actor() {
}

Actor::~Actor() {
}

void Actor::print() {
  printf("%s", id.c_str());
}

CollisionActor::CollisionActor() {
}

CollisionActor::~CollisionActor() {
}

void CollisionActor::update(float d_t) {
  while (!moveConstraints.empty()) {
    if (fabs(moveVector.x) > fabs(moveConstraints.front().x)) {
      moveVector.x = moveConstraints.front().x;
    }
    if (fabs(moveVector.y) > fabs(moveConstraints.front().y)) {
      moveVector.y = moveConstraints.front().y;
    }
    moveConstraints.pop();
  }
  sceneNode->translate(to3D(moveVector, sceneNode->getPosition().z));
  collisionShape.translate(to2D(sceneNode->getPosition()));
}

void CollisionActor::teleport(float x, float y) {
  teleport(Ogre::Vector2(x, y));
}

void CollisionActor::teleport(Ogre::Vector2 to) {
  this->moveVector = Ogre::Vector2::ZERO;
  sceneNode->setPosition(to3D(to, sceneNode->getPosition().z));
  collisionShape.translate(to2D(sceneNode->getPosition()));
}

void CollisionActor::move(float x, float y) {
  move(Ogre::Vector2(x, y));
}

void CollisionActor::move(Ogre::Vector2 by) {
  this->moveVector = by;
}
