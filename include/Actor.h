/*
 * Actor.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef ACTOR_H_
#define ACTOR_H_
#include "OGRE/Ogre.h"

class Actor {
 public:
  Actor(
      int actorID,
      int typeID,
      int shapeID,
      Ogre::SceneNode *sceneNode);

  virtual ~Actor();
    int getActorID() const;

    Ogre::SceneNode *getSceneNode() const;
    int getShapeID() const;
    int getTypeID() const;


 private:
  int actorID;
  int typeID;
  int shapeID;
  Ogre::SceneNode* sceneNode;
};

#endif /* ACTOR_H_ */
