/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"

bool Interaction<Character, Terrain>::collisionTest() {
  Ogre::Vector2 intersectionPoint;
  float a, b;
  return CollisionHandler::IntersectLineLine(
      first->getPosition(),
      first->getPosition() - first->oldPosition,
      second->getPosition(),
      second->collisionShape,
      *intersectionPoint, *a, *b);
}

void Interaction<Character, Terrain>::onInit() {}

void Interaction<Character, Terrain>::onEnter() {
  // std::cout << " " << u;
}

void Interaction<Character, Terrain>::onLeave() {}

void Interaction<Character, Terrain>::whileInside() {
  std::cout << "inside";
}

void Interaction<Character, Terrain>::whileOutside() {
  std::cout << "outside";
  first->move(Ogre::Vector2(0.0, -0.01));
}
