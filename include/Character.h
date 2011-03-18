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

class Character: public Actor {
 public:
  Character();
  virtual ~Character();
  void print();

  Ogre::Vector2 collisionShape;
};
#endif /* CHARACTER_H_ */
