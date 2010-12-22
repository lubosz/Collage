/*
 * AABBCollisioShape.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef AABBCOLLISIOSHAPE_H_
#define AABBCOLLISIOSHAPE_H_
#include "OGRE/Ogre.h"
#include "CollisionShape.h"

class AABBCollisioShape : public CollisionShape{
public:
	AABBCollisioShape(Ogre::Vector2 pos, Ogre::Vector2 ext);
	virtual ~AABBCollisioShape();
};

#endif /* AABBCOLLISIOSHAPE_H_ */
