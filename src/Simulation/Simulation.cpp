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
  character1->setPosition(Ogre::Vector2(0.0, 0.0));

  Terrain* terrain1 = t->createActor(rootSceneNode->createChildSceneNode());
  terrain1->collisionShape = Ogre::Vector2(1.0, 0.0);
  terrain1->sceneNode->setPosition(-0.5, -0.5, 0.0);
//  Terrain* terrain2 = t->createActor(rootSceneNode->createChildSceneNode());
//  Terrain* terrain3 = t->createActor(rootSceneNode->createChildSceneNode());
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
