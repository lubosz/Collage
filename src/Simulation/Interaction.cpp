/*
 *  Copyright 2011 The Collage Project
 */
#include "Interaction.h"

Interaction::Interaction(Actor* a, Actor* b) {
  this->a = a;
  this->b = b;
}

Interaction::~Interaction() {
}

void Interaction::enter() {
#ifdef DEBUG_OUTPUT
  printf("A%i, A%i -> enter (no specific interaction)",
      a->getActorID(),
      b->getActorID());
#endif
}

void Interaction::leave() {
#ifdef DEBUG_OUTPUT
  printf("A%i, A%i -> leave (no specific interaction)",
      a->getActorID(),
      b->getActorID());
#endif
}

void Interaction::inside(float d_t) {
#ifdef DEBUG_OUTPUT_TRIGGERED
  printf("A%i, A%i -> inside (no specific interaction)",
      a->getActorID(),
      b->getActorID());
#endif
}
