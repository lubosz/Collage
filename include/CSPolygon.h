/*
 * CSPolygon.h
 *
 *  Created on: 17.01.2011
 *      Author: lochmann
 *  Copyright 2011 The Collage Project
 */

#ifndef CSPOLYGON_H_
#define CSPOLYGON_H_

#include "CollisionShape.h"

class CSPolygon: public CollisionShape {
 public:
  CSPolygon();
  virtual ~CSPolygon();

  std::list<Ogre::Vector2> vertices;
};

#endif /* CSPOLYGON_H_ */
