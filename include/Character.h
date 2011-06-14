/*
 * Character.h
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Actor.h"

class Character: public CollisionActor {
 public:
  Character();
  virtual ~Character();
  void init();
  void manipulate(float d_t);

  bool test;
  Ogre::Vector2 velocity;
};
#endif /* CHARACTER_H_ */
