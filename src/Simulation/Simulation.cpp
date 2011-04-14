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
#include "ActorFactory.h"

#include <sstream>

Simulation::Simulation(Ogre::SceneManager *sceneManager, float frequency) {
  this->sceneManager = sceneManager;
	this->rootSceneNode = sceneManager->getRootSceneNode();
	this->frequency = frequency;
	d_t = 0.0;

  ActorFactory<Character> *characterFactory =
      new ActorFactory<Character>(sceneManager, "character");
  actorFactories.push_back(characterFactory);

  ActorFactory<Terrain> *terrainFactory =
      new ActorFactory<Terrain>(sceneManager, "terrain");
  actorFactories.push_back(terrainFactory);

  ActorFactory<Door> *doorFactory =
      new ActorFactory<Door>(sceneManager, "door");
  actorFactories.push_back(doorFactory);

  InteractionHandler<Character, Terrain>* ihCharacterTerrain =
    new InteractionHandler<Character, Terrain>(characterFactory,
                                               terrainFactory);
  interactionHandlers.push_back(
    static_cast<AbstractInteractionHandler*>(ihCharacterTerrain));

  InteractionHandler<Terrain, Door>* ihTerrainDoor =
    new InteractionHandler<Terrain, Door>(terrainFactory, doorFactory);
  interactionHandlers.push_back(
    static_cast<AbstractInteractionHandler*>(ihTerrainDoor));

  InteractionHandler<Character, Door>* ihCharacterDoor =
    new InteractionHandler<Character, Door>(characterFactory, doorFactory);
  interactionHandlers.push_back(
    static_cast<AbstractInteractionHandler*>(ihCharacterDoor));

  Character* character1 = characterFactory->createActor(
    rootSceneNode->createChildSceneNode());

  Terrain* terrain1 = terrainFactory->createActor(
    rootSceneNode->createChildSceneNode());
  Terrain* terrain2 = terrainFactory->createActor(
    rootSceneNode->createChildSceneNode());

  character1->teleport(0.0, 5.0);
  terrain1->teleport(-0.5, -0.5);
  terrain2->teleport(-1.5, 0.0);
}

Simulation::~Simulation() {}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
    foreach(AbstractActorFactory* a, actorFactories) {
      a->manipulate(d_t);
    }
    foreach(AbstractInteractionHandler* a, interactionHandlers) {
      a->update(d_t);
    }
    foreach(AbstractActorFactory* a, actorFactories) {
      a->update(d_t);
    }

    d_t = 0.0;
  }
}
