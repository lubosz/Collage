/*
 * AABBCollisioShape.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef AABBCOLLISIOSHAPE_H_
#define AABBCOLLISIOSHAPE_H_
#include "CollisionShape.h"

class AABBCollisioShape : public CollisionShape {
 public:
	AABBCollisioShape(Ogre::Vector2 pos, Ogre::Vector2 ext);
	virtual ~AABBCollisioShape();

	void attachDebugVisualization(Ogre::SceneNode *sceneNode);

 private:
	Ogre::Vector2 mPos;
	Ogre::Vector2 mExt;
};

#endif /* AABBCOLLISIOSHAPE_H_ */
