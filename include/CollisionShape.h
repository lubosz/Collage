/*
 * CollisionShape.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef COLLISIONSHAPE_H_
#define COLLISIONSHAPE_H_
#include "AABB.h"
#include "OGRE/Ogre.h"

class CollisionShape {
public:
	CollisionShape();
	virtual ~CollisionShape();

	virtual void attachDebugVisualization(Ogre::SceneNode *sceneNode) = 0;

	AABB* getAABB();

private:
	AABB mAABB;
};

#endif /* COLLISIONSHAPE_H_ */
