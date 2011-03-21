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
  void move(Ogre::Vector2 vec);

  Ogre::Vector2 oldPosition;
};
#endif /* CHARACTER_H_ */
