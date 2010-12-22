/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "Simulation.h"

Simulation::Simulation() {
	// TODO Auto-generated constructor stub
	mAABBList = new AABBList();

	mAABBList->push_back(AABB(Ogre::Vector2(0.0,0.0),Ogre::Vector2(1.0,1.0)));
	mAABBList->push_back(AABB(Ogre::Vector2(1.5,0.0),Ogre::Vector2(1.0,1.0)));
	mAABBList->push_back(AABB(Ogre::Vector2(0.0,1.5),Ogre::Vector2(1.0,1.0)));
}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
}

