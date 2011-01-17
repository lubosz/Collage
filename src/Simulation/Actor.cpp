/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Actor.h"


Actor::Actor(
    int actorID,
    int typeID,
    int shapeID,
    Ogre::SceneNode *sceneNode) {
  this->actorID = actorID;
  this->typeID = typeID;
  this->shapeID = shapeID;
  this->sceneNode = sceneNode;
  this->velocity = Ogre::Vector3::ZERO;
  this->totalVelocity = Ogre::Vector3::ZERO;
  this->translation = Ogre::Vector3::ZERO;

#ifdef DEBUG_OUTPUT
  std::cout<<
      "Created Actor: A" << actorID <<
      ", typeID:" << typeID <<
      ", shapeID: " << shapeID <<
      std::endl;
#endif
}

Actor::~Actor() {
}

int Actor::getActorID() const {
    return actorID;
}

int Actor::getShapeID() const {
    return shapeID;
}

int Actor::getTypeID() const {
    return typeID;
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
