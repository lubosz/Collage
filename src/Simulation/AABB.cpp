/*
 * AABB.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "AABB.h"

AABB::AABB(Ogre::Vector2 pos, Ogre::Vector2 ext) {
	this->pos = pos;
	this->ext = ext;
}

AABB::~AABB() {
	// TODO Auto-generated destructor stub
}

Ogre::Vector2 AABB::getCenter() const
{
	Ogre::Vector2 extHalf = getExt() / 2;
    return pos + extHalf;
}

Ogre::Vector2 AABB::getExt() const
{
    return ext;
}

Ogre::Vector2 AABB::getMax() const
{
    return pos + ext;
}

Ogre::Vector2 AABB::getPos() const
{
    return pos;
}

void AABB::setCenter(Ogre::Vector2 center)
{
    this->pos = center - getExt() / 2;
}

void AABB::setExtPivotPos(Ogre::Vector2 ext){
    this->ext = ext;
}

void AABB::scaleExtPivotPos(Ogre::Vector2 scale){
    this->ext = ext*scale;
}

void AABB::setPos(Ogre::Vector2 pos)
{
    this->pos = pos;
}


