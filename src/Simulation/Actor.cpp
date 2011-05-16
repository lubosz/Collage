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
  velocity = Ogre::Vector2::ZERO;
  possibleMoveRatio = 1.0;
  moveConstraintMin = Ogre::Vector2::ZERO;
  moveConstraintMax = Ogre::Vector2::ZERO;
}

CollisionActor::~CollisionActor() {
}

void CollisionActor::setValues(float d_t) {
  manipulate(d_t);
  moveConstraintMin = Ogre::Vector2::ZERO;
  moveConstraintMax = Ogre::Vector2::ZERO;
}

void CollisionActor::update(float d_t) {
  moveVector *= possibleMoveRatio;
  sceneNode->translate(to3D(moveVector, sceneNode->getPosition().z));
  collisionShape.translate(to2D(sceneNode->getPosition()));
  velocity = moveVector / d_t;
  moveVector = Ogre::Vector2::ZERO;
  possibleMoveRatio = 1.0;
}

CollisionActor* CollisionActor::teleport(float x, float y) {
  teleport(Ogre::Vector2(x, y));
  return this;
}

CollisionActor* CollisionActor::teleport(Ogre::Vector2 to) {
  this->moveVector = Ogre::Vector2::ZERO;
  sceneNode->setPosition(to3D(to, sceneNode->getPosition().z));
  collisionShape.translate(to2D(sceneNode->getPosition()));
  return this;
}

CollisionActor* CollisionActor::move(float x, float y) {
  move(Ogre::Vector2(x, y));
  return this;
}

CollisionActor* CollisionActor::move(Ogre::Vector2 by) {
  this->moveVector = by;
  return this;
}

CollisionActor* CollisionActor::constrainMove(Ogre::Vector2 by) {
  by.normalise();
  if (moveConstraintMin.isZeroLength()) {
    moveConstraintMin = by;
    moveConstraintMax = by;
  } else {
  // TODO(Gerrit) more then one constraint
  //    set min
  //    if v.rightTo(min)
  //      if v.rightTo(max)
  //        min = v
  //      else
  //        close
  //    else
  //      if v.rightTo(max)
  //        max = v
  //      else
  //        doNothing
    if (by.x * moveConstraintMin.y - by.y * moveConstraintMin.x > 0.0) {
      if (by.x * moveConstraintMax.y - by.y * moveConstraintMax.x > 0.0) {
        moveConstraintMin = by;
      } else {
        possibleMoveRatio = 0.0;
      }
    } else {
      if (by.x * moveConstraintMax.y - by.y * moveConstraintMax.x < 0.0) {
        moveConstraintMax = by;
      }
    }
  }
//  std::cout << moveConstraintMin << moveConstraintMax << std::endl;
  return this;
}

CollisionActor* CollisionActor::addPoint(float x, float y) {
  Ogre::Vector2 point(x, y);
  addPoint(point);
  return this;
}

CollisionActor* CollisionActor::addPoint(Ogre::Vector2 point) {
  collisionShapePoints.push_back(point);
  return this;
}

CollisionActor* CollisionActor::createCollisionShape(
    CollisionShape2::DefinedBy definedBy) {
  collisionShape.createShape(collisionShapePoints, definedBy);
  collisionShapePoints.clear();
  return this;
}
