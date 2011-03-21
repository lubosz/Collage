/*
 *  Copyright 2011 The Collage Project
 */

#ifndef INTERACTIONCHARACTERDOOR_H_
#define INTERACTIONCHARACTERDOOR_H_

#include "Interaction.h"
#include "Character.h"
#include "Door.h"

template<> class Interaction<Character, Door>
: public AbstractCollisionInteraction<Character, Door> {
 public:
  bool collisionTest();

  void onInit();

  void onEnter();

  void onLeave();

  void whileInside();

  void whileOutside();
};
#endif
