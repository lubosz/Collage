/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"

bool Interaction<Character, Terrain>::collisionTest() {
  Ogre::Vector2 p1 = first->getPosition();
  Ogre::Vector2 p2 = second->getPosition();

  Ogre::Vector2 d1 = p1 - first->oldPosition;
  Ogre::Vector2 d2 = second->collisionShape;

  // collision test
  if ((d1.x * (p2.y-p1.y) - d1.y * (p2.x-p1.x)) *
      (d1.x * p2.y-p1.y+d2.y - d1.y * p2.x-p1.x+d2.x) > 0.0) return false;

  if ((d2.x * (p1.y-p2.y) - d2.y * (p1.x-p2.x)) *
      (d2.x * p1.y-p2.y+d1.y - d2.y * p1.x-p2.x+d1.x) > 0.0) return false;

  return true;
}

void Interaction<Character, Terrain>::onInit() {}

void Interaction<Character, Terrain>::onEnter() {
//  std::cout << "entering";
  Ogre::Vector2 p1 = first->getPosition();
  Ogre::Vector2 p2 = second->getPosition();

  Ogre::Vector2 d1 = p1 - first->oldPosition;
  Ogre::Vector2 d2 = second->collisionShape;

  // calculate u-coord on the terrain vector
  // TODO(gerrit) bugfix necessary. result is -inf.
  float u = ((p2.x - p1.x) / d2.x + (p1.y - p2.y) / d2.y) *
      (d1.x * d2.y - d1.y *  d2.x) / (d2.x * d2.y);
  std::cout << " " << u;
}

void Interaction<Character, Terrain>::onLeave() {}

void Interaction<Character, Terrain>::whileInside() {
//  std::cout << "inside";
}

void Interaction<Character, Terrain>::whileOutside() {
//  std::cout << "outside";
  first->move(Ogre::Vector2(0.0, -0.01));
}
