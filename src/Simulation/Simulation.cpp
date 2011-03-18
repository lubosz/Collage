/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Simulation.h"
#include "Character.h"
#include "Terrain.h"

#include <sstream>

Simulation::Simulation(Ogre::SceneNode *rootSceneNode, float frequency) {
	this->rootSceneNode = rootSceneNode;
	this->frequency = frequency;
	d_t = 0.0;

  ActorFactory<Character>* c = new ActorFactory<Character>();
  ActorFactory<Terrain>* t = new ActorFactory<Terrain>();
  InteractionHandler<Character, Terrain>* ihCharacterTerrain =
      new InteractionHandler<Character, Terrain>(c, t);
  interactionHandlers.push_back(
      static_cast<AbstractInteractionHandler*>(ihCharacterTerrain));


  Character* character1 = c->createActor(rootSceneNode->createChildSceneNode());
  Character* character2 = c->createActor(rootSceneNode->createChildSceneNode());

  Terrain* terrain1 = t->createActor(rootSceneNode->createChildSceneNode());
  Terrain* terrain2 = t->createActor(rootSceneNode->createChildSceneNode());

  ihCharacterTerrain->print();
  std::cout << std::endl;

//  InteractionHandler<Character, Terrain>*
//  ihCharacterTerrain2 = new InteractionHandler<Character, Terrain>(c, t);
//
//  ihCharacterTerrain2->print();
//  std::cout << std::endl;
//  Terrain* terrain3 = t->createActor();
//  Terrain* terrain4 = t->createActor();

//  ihCharacterTerrain2->print();
//  std::cout << std::endl;
}

Simulation::~Simulation() {}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
    foreach(AbstractActorFactory* a, actorFactories) {
      a->update(d_t);
    }
    foreach(AbstractInteractionHandler* a, interactionHandlers) {
      std::cout << std::endl;
      a->update(d_t);
    }

    d_t = 0.0;
  }
}
