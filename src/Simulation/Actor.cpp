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
}

CollisionActor::~CollisionActor() {
}

void CollisionActor::update(float d_t) {
  sceneNode->translate(to3D(moveVector, sceneNode->getPosition().z));
  collisionShape.translate(to2D(sceneNode->getPosition()));
  velocity = moveVector / d_t;
  moveVector = Ogre::Vector2::ZERO;
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
  std::cout << by << moveVector << std::endl;
  if (fabs(moveVector.x) > fabs(by.x)) {
    moveVector.x = by.x;
  }
  if (fabs(moveVector.y) > fabs(by.y)) {
    moveVector.y = by.y;
  }
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
