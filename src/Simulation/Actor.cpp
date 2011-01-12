/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Actor.h"
#include "Simulation.h"

Actor::Actor(ActorBehavior behavior, Ogre::SceneNode *sceneNode,
    CollisionShape *collisionShape, float mass, float drag) {
	mMass = mass;
	mDrag = drag;
	mBehavior = behavior;
	mSceneNode = sceneNode;
	mCollisionShape = collisionShape;
}

Actor::~Actor() {
	// TODO(gerrit): Auto-generated destructor stub
}

void Actor::update(float d_t) {
	mSceneNode->translate(0.0, -0.001 * d_t, 0.0);
}

CollisionShape *Actor::getCollisionShape() const {
    return mCollisionShape;
}

ActorBehavior Actor::getActorBehavior() const {
    return mBehavior;
}

void Actor::setBehavior(ActorBehavior mBehavior) {
    this->mBehavior = mBehavior;
}

Ogre::SceneNode *Actor::getSceneNode() const {
    return mSceneNode;
}

AABB* Actor::getAABB() {
	return mCollisionShape->getAABB();
}

void Actor::drawDebugVisualization() {
	mCollisionShape->attachDebugVisualization(mSceneNode);
}
