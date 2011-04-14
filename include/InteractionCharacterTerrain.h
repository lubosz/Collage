/*
 *  Copyright 2011 The Collage Project
 */

#ifndef INTERACTIONCHARACTERTERRAIN_H_
#define INTERACTIONCHARACTERTERRAIN_H_

#include "Interaction.h"
#include "Character.h"
#include "Terrain.h"

template<> class Interaction<Character, Terrain>
: public AbstractCollisionInteraction<Character, Terrain> {
 public:
  Interaction();

  ~Interaction();

  bool collisionTest();

  void onInit();

  void onEnter();

  void onLeave();

  void whileInside();

  void whileOutside();
};
#endif /* INTERACTIONCHARACTERTERRAIN_H_ */
