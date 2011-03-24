/*
 *  Copyright 2010 The Collage Project
 */

#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() {
}

CollisionHandler::~CollisionHandler() {
}

inline void CollisionHandler::IntersectLineLine_scalars(
    Ogre::Vector2 pos1,
    Ogre::Vector2 dir1,
    Ogre::Vector2 pos2,
    Ogre::Vector2 dir2,
    float* scal1,
    float* scal2) {
  float quot = dir1.y * dir2.x - dir1.x * dir2.y;
  *scal1 = ((pos2.y - pos1.y) * dir2.x + (pos1.x - pos2.x) * dir2.y)/quot;
  *scal2 = ((pos1.y - pos2.y) * dir1.x + (pos2.x - pos1.x) * dir1.y)/quot;
}

inline bool CollisionHandler::IntersectLineLine(
    Ogre::Vector2 pos1,
    Ogre::Vector2 dir1,
    Ogre::Vector2 pos2,
    Ogre::Vector2 dir2,
    Ogre::Vector2* intersection,
    float* scal1,
    float* scal2) {
  IntersectLineLine_scalars(pos1, dir1, pos2, dir2, scal1, scal2);
  if (*scal1 > 1.0 || *scal2 > 1.0 || *scal1 * *scal2 < 0.0) {
    *intersection = pos1 + dir1 * *scal1;
    return true;
  }
  return false;
}
