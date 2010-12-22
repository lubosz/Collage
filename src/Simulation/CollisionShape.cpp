/*
 * CollisionShape.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "CollisionShape.h"

CollisionShape::CollisionShape() {
	// TODO Auto-generated constructor stub

}

CollisionShape::~CollisionShape() {
	// TODO Auto-generated destructor stub
}

AABB* CollisionShape::getAABB(){
	return &mAABB;
}
