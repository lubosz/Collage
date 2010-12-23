/*
 * AABB.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef AABB_H_
#define AABB_H_
#include "OGRE/OgreVector2.h"

class AABB {
public:
	AABB();
	AABB(Ogre::Vector2 pos, Ogre::Vector2 ext);
	virtual ~AABB();
    Ogre::Vector2 getCenter() const;
    Ogre::Vector2 getExt() const;
    Ogre::Vector2 getMax() const;
    Ogre::Vector2 getPos() const;
    void setCenter(Ogre::Vector2 center);
    void setExtPivotPos(Ogre::Vector2 ext);
    void scaleExtPivotPos(Ogre::Vector2 scale);
    void setPos(Ogre::Vector2 pos);



private:
	Ogre::Vector2 pos;
	Ogre::Vector2 ext;
};

#endif /* AABB_H_ */
