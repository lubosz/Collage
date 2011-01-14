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
    Actor *getA() const;
    Actor *getB() const;
    bool getOk() const;
    void setOk(bool ok);

 private:
  bool ok;
  Actor* a;
  Actor* b;
};

#endif /* INTERACTION_H_ */
