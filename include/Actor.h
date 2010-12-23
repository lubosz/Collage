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

//class Simulation;

enum ActorBehavior{
	AB_DYNAMIC = 0,
	AB_STATIC = 1,
	AB_TRIGGER = 2
};


class Actor{
public:
	Actor(ActorBehavior behavior, Ogre::SceneNode *sceneNode, float mass = 0.0, float drag = 0.0);
	virtual ~Actor();

	void update(float d_t);

	CollisionShape *getCollisionShape() const;
    void setCollisionShape(CollisionShape *mCollisionShape);
    ActorBehavior getActorBehavior() const;
    void setBehavior(ActorBehavior mBehavior);
    Ogre::SceneNode *getSceneNode() const;
    AABB* getAABB();

private:

    //Simulation *mSimulation;

    ActorBehavior mBehavior;
    CollisionShape *mCollisionShape;
    Ogre::SceneNode *mSceneNode;
    float mMass;
    float mDrag;
    Ogre::Vector2 velocity;
};

#endif /* ACTOR_H_ */
