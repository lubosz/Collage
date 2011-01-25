/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Simulation.h"
#include <sstream>

Simulation::Simulation(Ogre::SceneNode *rootSceneNode, float frequency) {
	this->rootSceneNode = rootSceneNode;
	this->frequency = frequency;
	d_t = 0.0;

  ActorFactory<Character>* c = new ActorFactory<Character>();
  c->createActor();

  ActorFactory<Terrain>* t = new ActorFactory<Terrain>();
  t->createActor();

  ihCharacterTerrain = new InteractionHandler<Character, Terrain>();
  ihCharacterTerrain->syncInteractions(c, t);
}

Simulation::~Simulation() {}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
    d_t = 0.0;
  }
}
