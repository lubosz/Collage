/*
 *  Copyright 2010 The Collage Project
 */

#include "DefaultSimulation.h"

DefaultSimulation::DefaultSimulation(Ogre::SceneManager* sceneManager)
  : Simulation(sceneManager, 60.0) {
  characterFactory = new ActorFactory<Character>(sceneManager, "character");
  actorFactories.push_back(characterFactory);

  terrainFactory = new ActorFactory<Terrain>(sceneManager, "terrain");
  actorFactories.push_back(terrainFactory);

  doorFactory = new ActorFactory<Door>(sceneManager, "door");
  actorFactories.push_back(doorFactory);

  hoverplaneFactory = new ActorFactory<Hoverplane>(sceneManager, "hoverplane");
  actorFactories.push_back(hoverplaneFactory);

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
}

DefaultSimulation::~DefaultSimulation() {
}
