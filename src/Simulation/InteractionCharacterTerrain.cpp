/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"
#include "CollisionHandler.h"

Interaction<Character, Terrain>::Interaction() {
}

Interaction<Character, Terrain>::~Interaction() {}

bool Interaction<Character, Terrain>::collisionTest() {
  Ogre::Vector2 moveConstraint = first->moveVector;
  if (CollisionHandler::collisionTestAABB(
      &first->collisionShape, &second->collisionShape,
      &moveConstraint, true)) {
//    std::cout << moveConstraint.x << " " << moveConstraint.y << std::endl;
//    first->constrainMove(moveConstraint);
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
  first->move(0., 1. * d_t);
}

void Interaction<Character, Terrain>::whileOutside(float d_t) {
}
