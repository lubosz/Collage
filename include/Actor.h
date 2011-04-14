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
#include "CollisionShape.h"

#define DEBUG_OUTPUT
// #define DEBUG_OUTPUT_TRIGGERED

class Actor {
 public:
  Actor();
  virtual ~Actor();

  std::string id;

  virtual void init() = 0;
  virtual void manipulate(float d_t) = 0;
  virtual void update(float d_t) = 0;
  void print();
};

class CollisionActor : public Actor {
 public:
  CollisionActor();
  virtual ~CollisionActor();

  Ogre::SceneManager* debugRendererSceneManager;
  Ogre::SceneNode* sceneNode;
  CollisionShape2 collisionShape;
  Ogre::Vector2 moveVector;

  virtual void init() = 0;
  virtual void manipulate(float d_t) = 0;
  void update(float d_t);

  void teleport(float x, float y);
  void teleport(Ogre::Vector2 to);
  void move(float x, float y);
  void move(Ogre::Vector2 by);
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
