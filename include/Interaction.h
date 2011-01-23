/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTION_H_
#define INTERACTION_H_

#include "Actor.h"

class Interaction {
 public:
  Interaction(Actor* a, Actor* b);
  virtual ~Interaction();
  bool ok;

  virtual void enter();
  virtual void leave();
  virtual void inside(float d_t);

 private:
  Actor* a;
  Actor* b;
};
#endif /* INTERACTION_H_ */
