/*
 *  Copyright 2010 The Collage Project
 */

#ifndef DEFAULTSIMULATION_H_
#define DEFAULTSIMULATION_H_
#include "Simulation.h"
#include "Door.h"
#include "Character.h"
#include "Terrain.h"

class DefaultSimulation : public Simulation{
public:
  DefaultSimulation(Ogre::SceneManager* sceneManager);
  virtual ~DefaultSimulation();

  ActorFactory<Terrain> *terrainFactory;
  ActorFactory<Character> *characterFactory;
  ActorFactory<Door> *doorFactory;
};

#endif /* DEFAULTSIMULATION_H_ */
