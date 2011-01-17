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

#define DEBUG_OUTPUT
#define DEBUG_OUTPUT_TRIGGERED

class Actor {
 public:
  Actor(
      int actorID,
      int typeID,
      int shapeID,
      Ogre::SceneNode *sceneNode);

  virtual ~Actor();

  // Identification
  int getActorID() const;
  int getShapeID() const;
  int getTypeID() const;

  // Animation
  Ogre::SceneNode *getSceneNode() const;
  Ogre::Vector3 getPosition();
  Ogre::Vector3 getVelocity();
  Ogre::Vector3 getAcceleration();
  void addVelocity(Ogre::Vector3 velocity);
  void addAcceleration(Ogre::Vector3 acceleration);
  void addMotionLock(Ogre::Vector2 wallNormal);
  void update(float d_t);

 private:
  // Identification
  int actorID;
  int typeID;
  int shapeID;

  // Animation
  Ogre::SceneNode* sceneNode;
  Ogre::Vector3 velocity;
  Ogre::Vector3 acceleration;
  std::queue<Ogre::Vector2> motionLocks;
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
