/*
 * Simulation.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "OGRE/Ogre.h"
#include "SimulationEnums.h"

class Simulation {
 public:
	Simulation(Ogre::SceneNode *rootSceneNode, float frequency);
	virtual ~Simulation();

	void update(float timeSinceLastFrame);

	Actor* createActor(
	    InteractionType actorType,
	    CollisionShape collisionShape,
			Ogre::Vector3 position = Ogre::Vector3::ZERO,
			bool isStatic = true,
			float rotation = 0.0,
			float scale = 1.0
	);

	void attachInteractionHandler(
	    InteractionType actorTypeA,
	    InteractionType actorTypeB,
	    InteractionHandler* interactionHandler
	);

	bool mDebugVisualization;


 private:
	float frequency;
	float d_t;

	Ogre::SceneNode *rootSceneNode;

	typedef std::vector<Actor*> ActorList;
  ActorList dynamicActors;
  ActorList staticActors;

  typedef std::pair<int, int> InteractionHandlerID;
  std::map<InteractionHandlerID, InteractionHandler*> interactionHandlers;

	int currentActorID;
	int generateActorID();

  int currentInteractionTypeID;
  int generateInteractionTypeID();

	void sortActorsByActorID(Actor** a, Actor** b);

	void sortActorsByTypeID(Actor** a, Actor** b);

	void sortActorsByShapeID(Actor** a, Actor** b);

	void sortInt(int* a, int* b);
};
#endif /* SIMULATION_H_ */
