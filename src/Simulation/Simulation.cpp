/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Simulation.h"
#include <QtCore>
#include <sstream>

#include "IHCharacterGravity.h"

Simulation::Simulation(Ogre::SceneNode *rootSceneNode) {
	this->rootSceneNode = rootSceneNode;
	currentActorID = 0;
}

Simulation::~Simulation() {}


Actor* Simulation::createActor(
    ActorType actorType,
    CollisionShape collisionShape,
    Ogre::Vector3 position,
    bool isStatic,
    float orientation,
    float scale) {
  int actorID = generateActorID();
  std::stringstream name;
  name << actorID;
	Ogre::SceneNode *sceneNode =
	    rootSceneNode->createChildSceneNode(name.str(), position);

	int shapeID = actorType;
  int typeID = collisionShape;

	Actor *actor = new Actor(actorID, typeID, shapeID, sceneNode);

	if(isStatic)
	  staticActors.push_back(actor);
	else
	  dynamicActors.push_back(actor);

	return actor;
}

void Simulation::attachInteractionHandler(
    ActorType actorTypeA,
    ActorType actorTypeB,
    InteractionHandler* interactionHandler) {
  int a = actorTypeA;
  int b = actorTypeB;
  sortInt(&a, &b);
  interactionHandlers[InteractionHandlerID(a, b)] = interactionHandler;
}

void Simulation::update(float d_t) {
  foreach(Actor* a, dynamicActors) {
    foreach(Actor* b, staticActors) {
      Actor* aTemp = a;
      Actor* bTemp = b;

      sortActorsByTypeID(&aTemp, &bTemp);
      InteractionHandlerID id =
          InteractionHandlerID(aTemp->getTypeID(), bTemp->getTypeID());
      std::map<InteractionHandlerID, InteractionHandler*>::iterator it =
          interactionHandlers.find(id);

#ifdef DEBUG_OUTPUT_TRIGGERED
      std::cout <<
          "Collision Detected between Actors" << aTemp->getActorID() <<
          " and " << bTemp->getActorID() <<
          std::endl;
      if (it == interactionHandlers.end()) {
        std::cout <<
            "InteractionHandler for Actortypes " << aTemp->getTypeID() <<
            " and " << bTemp->getTypeID() <<
            std::endl;
      } else {
        std::cout <<
            "No InteractionHandler for Actorstypes " << aTemp->getTypeID() <<
            " and " << bTemp->getTypeID() <<
            std::endl;
      }
#endif
    }
  }
}

int Simulation::generateActorID() {
  int ret = currentActorID;
  currentActorID++;
  return ret;
}

int Simulation::generateInteractionTypeID() {
  int ret = currentInteractionTypeID;
  currentInteractionTypeID++;
  return ret;
}

void Simulation::sortActorsByActorID(Actor** a, Actor** b) {
  if ((*a)->getActorID() < (*b)->getActorID()) return;
  Actor* c = *a;
  *a = *b;
  *b = c;
}

void Simulation::sortActorsByTypeID(Actor** a, Actor** b) {
  if ((*a)->getTypeID() < (*b)->getTypeID()) return;
  Actor* c = *a;
  *a = *b;
  *b = c;
}

void Simulation::sortActorsByShapeID(Actor** a, Actor** b) {
  if ((*a)->getShapeID() < (*b)->getShapeID()) return;
  Actor* c = *a;
  *a = *b;
  *b = c;
}

void Simulation::sortInt(int* a, int* b) {
  if (*a < *b) return;
  int c = *a;
  *a = *b;
  *b = c;
}

