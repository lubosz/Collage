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
  Character* character1 = c->createActor();

  ActorFactory<Terrain>* t = new ActorFactory<Terrain>();
  Terrain* terrain1 = t->createActor();

  ihCharacterTerrain = new InteractionHandler<Character, Terrain>(c, t);

  printf("current(");
  ihCharacterTerrain->print();
  printf(")\n");

  Character* character2 = c->createActor();

  printf("current(");
  ihCharacterTerrain->print();
  printf(")\n");
}

Simulation::~Simulation() {}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
    d_t = 0.0;
  }
}
