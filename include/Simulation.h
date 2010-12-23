/*
 * Simulation.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "AABB.h"
#include "OGRE/Ogre.h"
#include "Actor.h"

typedef std::vector<Actor*> ActorList;

class Simulation {
public:
	Simulation(Ogre::SceneNode *rootSceneNode);
	virtual ~Simulation();

	void update(float d_t);

	Actor* createActor(
			Ogre::String name,
			ActorBehavior behavior,
			Ogre::Vector2 position,
			CollisionShape *collisionShape = new AABBCollisioShape(Ogre::Vector2(0.0,0.0),Ogre::Vector2(1.0,1.0))
	);
    Ogre::Vector2 getGravity() const;
    void setGravity(Ogre::Vector2 mGravity);

	bool mDebugVisualization;

private:
	Ogre::SceneNode *mRootSceneNode;
	ActorList mActorListDynamic;
	ActorList mActorListStatic;

	Ogre::Vector2 mGravity;
};

#endif /* SIMULATION_H_ */
