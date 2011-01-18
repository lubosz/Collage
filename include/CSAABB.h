/*
 * CSAABB.h
 *
 *  Created on: 17.01.2011
 *      Author: lochmann
 *  Copyright 2011 The Collage Project
 */

#ifndef CSAABB_H_
#define CSAABB_H_

#include "CollisionShape.h"

class CSAABB : public CollisionShape {
 public:
  CSAABB();
  virtual ~CSAABB();

  Ogre::Vector2 aABB;
};

#endif /* CSAABB_H_ */
