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

  Ogre::SceneManager* sceneManager;
  Ogre::SceneNode* sceneNode;
  std::string id;

  virtual void init() = 0;
  virtual void setValues(float d_t) = 0;
  virtual void update(float d_t) = 0;
  void print();
};

class CollisionActor : public Actor {
 public:
  CollisionActor();
  virtual ~CollisionActor();

  std::vector<Ogre::Vector2> collisionShapePoints;
  CollisionShape2 collisionShape;
  Ogre::Vector2 moveVector;
  float possibleMoveRatio;
  Ogre::Vector2 moveConstraintMin;
  Ogre::Vector2 moveConstraintMax;

  virtual void init() = 0;
  virtual void manipulate(float d_t) = 0;
  virtual void setValues(float d_t);
  void update(float d_t);

  CollisionActor* teleport(float x, float y);
  CollisionActor* teleport(Ogre::Vector2 to);
  CollisionActor* move(float x, float y);
  CollisionActor* move(Ogre::Vector2 by);
  CollisionActor* constrainMove(Ogre::Vector2 by);
  CollisionActor* addPoint(float x, float y);
  CollisionActor* addPoint(Ogre::Vector2 point);
  CollisionActor* createCollisionShape(CollisionShape2::DefinedBy definedBy);
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
