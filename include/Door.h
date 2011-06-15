/*
 * Door.h
 *
 *  Created on: 24.01.2011
 *      Author: Dein Vater
 *  Copyright 2010 The Collage Project
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "Actor.h"

class Door: public CollisionActor {
 public:
  Door();
  virtual ~Door();
  void init();
  void update(float d_t) {}
  void manipulate(float d_t);
  QRect geometry;
};

#endif
