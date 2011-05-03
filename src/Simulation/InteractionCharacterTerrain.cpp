/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"
#include "CollisionHandler.h"

Interaction<Character, Terrain>::Interaction() {
}

Interaction<Character, Terrain>::~Interaction() {}

bool Interaction<Character, Terrain>::collisionTest() {
  float ratio;
  Ogre::Vector2 minCollisionNormal;
  Ogre::Vector2 maxCollisionNormal;
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
      &first->collisionShape.aabb,
      &second->collisionShape.linestrip,
      &first->moveVector, &ratio, &minCollisionNormal, &maxCollisionNormal)) {
    first->moveVector *= ratio;
    first->constrainMove(minCollisionNormal);
    return true;
  }
  return false;
}

void Interaction<Character, Terrain>::onInit() {}

void Interaction<Character, Terrain>::onEnter() {
  std::cout << first->id << " entering " << second->id << std::endl;
}

void Interaction<Character, Terrain>::onLeave() {
  std::cout << first->id << " leaving " << second->id << std::endl;
}

void Interaction<Character, Terrain>::whileInside(float d_t) {
}

void Interaction<Character, Terrain>::whileOutside(float d_t) {
}
