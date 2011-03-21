/*
 *  Copyright 2011 The Collage Project
 */

#ifndef INTERACTIONCHARACTERTERRAIN_H_
#define INTERACTIONCHARACTERTERRAIN_H_

#include "Interaction.h"
#include "Character.h"
#include "Terrain.h"

template<> class Interaction<Character, Terrain>
: public AbstractInteraction<Character, Terrain> {
 public:
  float test;

  virtual void init();
  virtual void interact();
};
#endif /* INTERACTIONCHARACTERTERRAIN_H_ */
