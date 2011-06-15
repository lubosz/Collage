/*
 * Hoverplane.h
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef HOVERPLANE_H_
#define HOVERPLANE_H_

#include "Actor.h"

class Hoverplane: public CollisionActor {
 public:
  Hoverplane();
  virtual ~Hoverplane();
  void init();
  void manipulate(float d_t);

  Hoverplane* setPath(float x1, float y1, float x2, float y2);
  Hoverplane* setSpeed(float speed);
  Ogre::Vector2 p1, p2;
  float speed;
  float u;
  bool incr;
};

#endif /* HOVERPLANE_H_ */
