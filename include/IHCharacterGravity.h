/*
 * IHCharacterGravity.h
 *
 *  Created on: 14.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef IHCHARACTERGRAVITY_H_
#define IHCHARACTERGRAVITY_H_

#include "InteractionHandler.h"

class IHCharacterGravity: public InteractionHandler {
 public:
  explicit IHCharacterGravity(Ogre::Vector2 gravity);
  virtual ~IHCharacterGravity();
//
//  void inside(Interaction* interaction, float d_t);
//
//  Ogre::Vector2 gravity;
};

#endif /* IHCHARACTERGRAVITY_H_ */
