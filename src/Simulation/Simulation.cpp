/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */
#include "Simulation.h"
#include "ActorFactory.h"

#include <sstream>

Simulation::Simulation(Ogre::SceneManager *sceneManager, float frequency) {
  this->sceneManager = sceneManager;
  this->rootSceneNode = sceneManager->getRootSceneNode();
  this->frequency = frequency;
  d_t = 0.0;
}

Simulation::~Simulation() {}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
    foreach(AbstractActorFactory* a, actorFactories) {
      a->setValues(d_t);
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
