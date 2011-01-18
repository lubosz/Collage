/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Actor.h"
#include <QtCore>

Actor::Actor(
    int actorID,
    int interactionTypeID,
    CollisionType collisionTypeID,
    Ogre::SceneNode *sceneNode) {
  this->actorID = actorID;
  this->interactionTypeID = interactionTypeID;
  this->collisionTypeID = collisionTypeID;
  this->sceneNode = sceneNode;
  this->velocity = Ogre::Vector3::ZERO;
  this->totalVelocity = Ogre::Vector3::ZERO;
  this->translation = Ogre::Vector3::ZERO;

  switch (collisionTypeID) {
  case CT_GLOBAL:
    this->collisionShape = NULL;
    break;
  case CT_POLYGON:
    this->collisionShape = new CSPolygon();
    break;
  case CT_AABB:
    this->collisionShape = new CSAABB();
    break;
  }

#ifdef DEBUG_OUTPUT
  std::cout<<
      "Created Actor: A" << actorID <<
      ", interactionTypeID:" << interactionTypeID <<
      ", collisionTypeID: " << collisionTypeID <<
      std::endl;
#endif
}

Actor::~Actor() {
}

int Actor::getActorID() const {
    return actorID;
}

int Actor::getCollisionTypeID() const {
    return collisionTypeID;
}

int Actor::getInteractionTypeID() const {
    return interactionTypeID;
}

Ogre::SceneNode *Actor::getSceneNode() const {
    return sceneNode;
}

Ogre::Vector3 Actor::getPosition() {
  return sceneNode->getPosition();
}

Ogre::Vector3 Actor::getTotalVelocity() {
  return totalVelocity;
}

void Actor::addVelocity(Ogre::Vector3 velocity) {
  this->velocity += velocity;
}

void Actor::addTranslation(Ogre::Vector3 translation) {
  this->translation += translation;
}

void Actor::addMotionLock(Ogre::Vector2 wallNormal) {
  motionLocks.push(wallNormal);
}

void Actor::update(float d_t) {
  translation += velocity * d_t;

  while (!motionLocks.empty()) {
    if (to2D(translation).dotProduct(motionLocks.front()) < 0.0) {
      translation = Ogre::Vector3::ZERO;
      velocity = Ogre::Vector3::ZERO;
    }
    motionLocks.pop();
  }

  sceneNode->translate(translation, Ogre::Node::TS_LOCAL);
  translation = Ogre::Vector3::ZERO;

#ifdef DEBUG_OUTPUT_TRIGGERED
  std::cout<<
      "Updated A" << actorID <<
      ", position: (" << getPosition().x <<
      ", " << getPosition().y <<
      ", " << getPosition().z << ")" <<
      std::endl;
#endif
}

CollisionShape* Actor::getCollisionShape() {
  return collisionShape;
}
