/*
 *  Copyright 2010 The Collage Project
 */

#ifndef COLLISIONHANDLER_H_
#define COLLISIONHANDLER_H_

#include "Ogre.h"

class CollisionHandler {
 public:
  CollisionHandler();
  virtual ~CollisionHandler();

  static inline void IntersectLineLine_scalars(
      Ogre::Vector2 pos1,
      Ogre::Vector2 pos2,
      Ogre::Vector2 dir1,
      Ogre::Vector2 dir2,
      float* scal1,
      float* scal2);

  static inline bool IntersectLineLine(
      Ogre::Vector2 pos1,
      Ogre::Vector2 pos2,
      Ogre::Vector2 dir1,
      Ogre::Vector2 dir2,
      Ogre::Vector2* intersection,
      float* scal1,
      float* scal2);
};

#endif /* COLLISIONHANDLER_H_ */
