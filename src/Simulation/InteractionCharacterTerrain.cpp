/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"

bool Interaction<Character, Terrain>::collisionTest(
    Character* first, Terrain* second) {
  Ogre::Vector2 p1 = first->getPosition();
  Ogre::Vector2 p2 = second->getPosition();

  Ogre::Vector2 d1 = p1 - first->oldPosition;
  Ogre::Vector2 d2 = second->collisionShape;

  if (!isCrossing(d1, p2-p1, p2-p1+d2)) return false;
  if (!isCrossing(d2, p1-p2, p1-p2+d1)) return false;
  return true;
}

bool Interaction<Character, Terrain>::isCrossing(
    Ogre::Vector2 vec, Ogre::Vector2 pnt1, Ogre::Vector2 pnt2) {
  float d1 = (vec.x * pnt1.y) - (vec.y * pnt1.x);
  float d2 = (vec.x * pnt2.y) - (vec.y * pnt2.x);
  if (d1 * d2 < 0.0) return true;
  return false;
}

void Interaction<Character, Terrain>::onInit() {}

void Interaction<Character, Terrain>::onEnter() {}

void Interaction<Character, Terrain>::onLeave() {}

void Interaction<Character, Terrain>::whileInside() {
  std::cout << "inside";
}

void Interaction<Character, Terrain>::whileOutside() {
  std::cout << "outside";
  first->move(Ogre::Vector2(0.0, -0.01));
}
