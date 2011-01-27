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

class Terrain: public Actor {
 public:
  Terrain();
  virtual ~Terrain();
  void print();
};

#endif /* TERRAIN_H_ */
