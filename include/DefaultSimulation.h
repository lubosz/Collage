/*
 *  Copyright 2010 The Collage Project
 */

#ifndef DEFAULTSIMULATION_H_
#define DEFAULTSIMULATION_H_
#include "Simulation.h"
#include "Door.h"
#include "Character.h"
#include "Terrain.h"
#include "Hoverplane.h"

class DefaultSimulation : public Simulation {
  public:
    explicit DefaultSimulation(Ogre::SceneManager* sceneManager);
    virtual ~DefaultSimulation();

    ActorFactory<Terrain> *terrainFactory;
    ActorFactory<Character> *characterFactory;
    ActorFactory<Door> *doorFactory;
    ActorFactory<Hoverplane> *hoverplaneFactory;
};

#endif /* DEFAULTSIMULATION_H_ */
