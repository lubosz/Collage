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
    first->moveConstraints.push(moveConstraint);
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

void Interaction<Character, Terrain>::whileInside() {
}

void Interaction<Character, Terrain>::whileOutside() {
}
