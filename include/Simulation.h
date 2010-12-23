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

	Actor* createActor(Ogre::String name, ActorBehavior behavior, Ogre::Vector2 position);

	bool mDebugVisualization;

private:
	Ogre::SceneNode *mRootSceneNode;
	ActorList mActorListDynamic;
	ActorList mActorListStatic;
};

#endif /* SIMULATION_H_ */
