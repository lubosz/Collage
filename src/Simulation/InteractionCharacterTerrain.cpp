/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"
#include "CollisionHandler.h"

Interaction<Character, Terrain>::Interaction() {
}

Interaction<Character, Terrain>::~Interaction() {}

bool Interaction<Character, Terrain>::collisionTest() {
  Ogre::Vector2 minCollisionNormal = Ogre::Vector2::ZERO;
  Ogre::Vector2 maxCollisionNormal = Ogre::Vector2::ZERO;
//  if (CollisionHandler::collisionTestAABB(
//      &first->collisionShape, &second->collisionShape,
//      &first->moveVector, &ratio, &collisionNormal)) {
//    first->moveVector *= ratio;
//    first->constrainMove(collisionNormal);
//    return true;
//  }
//  if (CollisionHandler::collisionTestAABBLine(
//      &first->collisionShape.aabb,
//      second->collisionShape.linestrip.points[0],
//      &second->collisionShape.linestrip.edges[0],
//      &first->moveVector, &ratio, &collisionNormal)) {
  if (CollisionHandler::collisionTestAABBLinestrip(
      &first->collisionShape,
      &second->collisionShape.linestrip,
      &first->moveVector,
      &first->possibleMoveRatio,
      &minCollisionNormal,
      &maxCollisionNormal)) {
//    std::cout << second->id;
//    printf(" RATIO: %f \n", first->possibleMoveRatio);
    first->constrainMove(minCollisionNormal);
    first->constrainMove(maxCollisionNormal);
    return true;
  }
  return false;
}

void Interaction<Character, Terrain>::onInit() {}

void Interaction<Character, Terrain>::onEnter() {
  std::cout << first->id << " entering " << second->id
      << "mcMin: " << first->moveConstraintMin
      << "mcMax: " << first->moveConstraintMax
      << "ratio: " << first->possibleMoveRatio
      << std::endl;
}

void Interaction<Character, Terrain>::onLeave() {
  std::cout << first->id << " leaving " << second->id << std::endl;
}

void Interaction<Character, Terrain>::whileInside(float d_t) {
}

void Interaction<Character, Terrain>::whileOutside(float d_t) {
}
