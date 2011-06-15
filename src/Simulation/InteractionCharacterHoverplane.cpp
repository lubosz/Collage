/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterHoverplane.h"
#include "CollisionHandler.h"

bool Interaction<Character, Hoverplane>::collisionTest() {
  Ogre::Vector2 relativeTranslation = first->moveVector - second->moveVector;
  Ogre::Vector2 collisionNormal = Ogre::Vector2::ZERO;
  if (CollisionHandler::collisionTestAABB(
      &first->collisionShape,
      &second->collisionShape,
      &relativeTranslation,
      &first->possibleMoveRatio,
      &collisionNormal)) {
    first->constrainMove(collisionNormal);
    return true;
  }
  return false;
}

void Interaction<Character, Hoverplane>::onInit() {}

void Interaction<Character, Hoverplane>::onEnter() {
  std::cout << first->id << " entering " << second->id << std::endl;
}

void Interaction<Character, Hoverplane>::onLeave() {
  std::cout << first->id << " leaving " << second->id << std::endl;
}

void Interaction<Character, Hoverplane>::whileInside(float d_t) {
  if (first->moveConstraintMin.y == 1.0 || first->moveConstraintMax.y == 1.0)
    first->moveVector += second->moveVector;
}

void Interaction<Character, Hoverplane>::whileOutside(float d_t) {
}
