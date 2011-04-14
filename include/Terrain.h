/*
 * Terrain.h
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Actor.h"

class Terrain: public CollisionActor {
 public:
  Terrain();
  virtual ~Terrain();
  void init();
  void manipulate(float d_t);
};

#endif /* TERRAIN_H_ */
