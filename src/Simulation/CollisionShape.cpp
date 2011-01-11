/*
 * CollisionShape.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "CollisionShape.h"

CollisionShape::CollisionShape() {}

CollisionShape::~CollisionShape() {}

AABB* CollisionShape::getAABB() {
	return &mAABB;
}
