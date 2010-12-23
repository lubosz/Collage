/*
 * Actor.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef ACTOR_H_
#define ACTOR_H_
#include "CollisionShape.h"
#include "OGRE/Ogre.h"

enum ActorBehavior{
	AB_STATIC = 1,
	AB_TRIGGER = 2
};

class Actor{
public:
	Actor(ActorBehavior behavior, Ogre::SceneNode *sceneNode);
	virtual ~Actor();

	CollisionShape *getCollisionShape() const;
    void setCollisionShape(CollisionShape *mCollisionShape);
    ActorBehavior getActorBehavior() const;
    void setBehavior(ActorBehavior mBehavior);
    Ogre::SceneNode *getSceneNode() const;
    AABB* getAABB();

private:

    ActorBehavior mBehavior;
    CollisionShape *mCollisionShape;
    Ogre::SceneNode *mSceneNode;
};

#endif /* ACTOR_H_ */
