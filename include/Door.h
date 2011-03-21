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

class Door: public Actor {
 public:
  Door();
  virtual ~Door();
  void print();

  Ogre::Vector2 collisionShape;
};

#endif
