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
#include <QtCore>

#define DEBUG_OUTPUT
// #define DEBUG_OUTPUT_TRIGGERED

class Actor {
 public:
  Actor();

  virtual ~Actor();

  // Animation
  Ogre::SceneNode *getSceneNode() const;

 private:
  // Animation
  Ogre::SceneNode* sceneNode;
};

inline static Ogre::Vector2 to2D(Ogre::Vector3 in) {
  return Ogre::Vector2(in.x, in.y);
}

inline static Ogre::Vector2 to2D(Ogre::Vector3 in, float *preserveZ) {
  *preserveZ = in.z;
  return Ogre::Vector2(in.x, in.y);
}

inline static Ogre::Vector3 to3D(Ogre::Vector2 in, float atZ = 0.0) {
  return Ogre::Vector3(in.x, in.y, atZ);
}

#endif /* ACTOR_H_ */
