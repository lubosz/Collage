/*
 * Simulation.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "OGRE/Ogre.h"
#include "InteractionHandler.h"

class Simulation {
public:
	Simulation(Ogre::SceneNode *rootSceneNode);
	virtual ~Simulation();

	void update(float d_t);

	Actor* createActor(
			std::string actorType,
			std::string collisionShape,
			Ogre::Vector3 position = Ogre::Vector3::ZERO,
			bool isStatic = true,
			float rotation = 0.0,
			float scale = 1.0
	);

	bool mDebugVisualization;

private:
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

	void sortActorsByActorID(Actor* a, Actor* b);

	void sortActorsTypeID(Actor* a, Actor* b);

	void sortActorsByShapeID(Actor* a, Actor* b);

	void sortInt(int* a, int* b);

}

#endif /* SIMULATION_H_ */
