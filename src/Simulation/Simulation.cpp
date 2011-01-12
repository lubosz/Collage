/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Simulation.h"
#include <QtCore>
#include "Actor.h"

Simulation::Simulation(Ogre::SceneNode *rootSceneNode) {
	mRootSceneNode = rootSceneNode;
}

Simulation::~Simulation() {}

Actor* Simulation::createActor(const Ogre::String name, ActorBehavior behavior,
    Ogre::Vector2 position, CollisionShape *collisionShape) {
	Ogre::SceneNode *sceneNode = mRootSceneNode->
	    createChildSceneNode(name, Ogre::Vector3(position.x, position.y, 0.0));
	Actor *actor = new Actor(behavior, sceneNode, collisionShape);
	if (behavior == AB_STATIC || behavior == AB_STATIC_TRIGGER) {
		mActorListStatic.push_back(actor);
	} else {
		mActorListDynamic.push_back(actor);
	}
	if(mDebugVisualization) {
		actor->drawDebugVisualization();
	}
	return actor;
}

void Simulation::update(float d_t) {
	foreach(Actor *dyn, mActorListDynamic) {
		dyn->update(d_t);
	}

	foreach(Actor *dyn, mActorListDynamic) {
		foreach(Actor *sta, mActorListStatic) {}
	}
}

Ogre::Vector2 Simulation::getGravity() const {
    return mGravity;
}

void Simulation::setGravity(Ogre::Vector2 mGravity) {
    this->mGravity = mGravity;
}
