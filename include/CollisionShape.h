/*
 * CollisionShape.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef COLLISIONSHAPE_H_
#define COLLISIONSHAPE_H_
#include "AABB.h"

class CollisionShape {
public:
	CollisionShape();
	virtual ~CollisionShape();

	AABB* getAABB();

private:
	AABB mAABB;
};

#endif /* COLLISIONSHAPE_H_ */
