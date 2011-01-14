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

#ifdef DEBUG_OUTPUT
  std::cout<<
      "Created Actor: " << actorID <<
      ", shapeID: " << shapeID <<
      ", typeID:" << typeID <<
      std::endl;
#endif
}

Actor::~Actor() {
}

int Actor::getActorID() const {
    return actorID;
}

Ogre::SceneNode *Actor::getSceneNode() const {
    return sceneNode;
}

int Actor::getShapeID() const {
    return shapeID;
}

int Actor::getTypeID() const {
    return typeID;
}
