/*
 *  Copyright 2011 The Collage Project
 */

#ifndef INTERACTIONCHARACTERHOVERPLANE_H_
#define INTERACTIONCHARACTERHOVERPLANE_H_

#include "Interaction.h"
#include "Character.h"
#include "Hoverplane.h"

template<> class Interaction<Character, Hoverplane>
: public AbstractCollisionInteraction<Character, Hoverplane> {
 public:
  bool collisionTest();

  void onInit();

  void onEnter();

  void onLeave();

  void whileInside(float d_t);

  void whileOutside(float d_t);
};
#endif /* INTERACTIONCHARACTERHOVERPLANE_H_ */
