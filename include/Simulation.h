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
#include "InteractionHandler.h"

class Simulation {
 public:
  Simulation();
	Simulation(Ogre::SceneNode *rootSceneNode, float frequency);
	virtual ~Simulation();

	void update(float timeSinceLastFrame);

	bool mDebugVisualization;

 private:
	float frequency;
	float d_t;

	Ogre::SceneNode *rootSceneNode;

	InteractionHandler<Character, Terrain>* ihCharacterTerrain;
};
#endif /* SIMULATION_H_ */
