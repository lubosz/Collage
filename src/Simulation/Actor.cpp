/*
 * Actor.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "Actor.h"

Actor::Actor(ActorBehavior behavior, Ogre::SceneNode *sceneNode) {
	mBehavior = behavior;
	mSceneNode = sceneNode;
	// TODO Auto-generated constructor stub

}

Actor::~Actor() {
	// TODO Auto-generated destructor stub
}

CollisionShape *Actor::getCollisionShape() const
{
    return mCollisionShape;
}

ActorBehavior Actor::getActorBehavior() const
{
    return mBehavior;
}

void Actor::setBehavior(ActorBehavior mBehavior)
{
    this->mBehavior = mBehavior;
}

Ogre::SceneNode *Actor::getSceneNode() const
{
    return mSceneNode;
}

AABB* Actor::getAABB(){
	return mCollisionShape->getAABB();
}

