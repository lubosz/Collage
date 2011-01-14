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
#define DEBUG_OUTPUT

Simulation::Simulation(Ogre::SceneNode *rootSceneNode) {
	this->rootSceneNode = rootSceneNode;
	currentActorID = 0;

	int a = 0;
	int b = 1;
	sortInt(&a, &b);
	interactionHandlers[InteractionHandlerID(a, b)] = new InteractionHandler();
}

Simulation::~Simulation() {}


Actor* Simulation::createActor(
    std::string actorType,
    std::string collisionShape,
    Ogre::Vector3 position,
    bool isStatic,
    float orientation,
    float scale) {
  int actorID = generateActorID();
  std::stringstream name;
  name << actorID;
	Ogre::SceneNode *sceneNode =
	    rootSceneNode->createChildSceneNode(name.str(), position);

	int shapeID = 0;
	if (collisionShape.compare("circle") == 0) shapeID = 1;
	if (collisionShape.compare("box") == 0) shapeID = 2;

  int typeID = 0;
  if (actorType.compare("character") == 0) typeID = 1;
  if (actorType.compare("item") == 0) typeID = 2;
  if (actorType.compare("terrain") == 0) typeID = 3;


	Actor *actor = new Actor(actorID, typeID, shapeID, sceneNode);

	if(isStatic)
	  staticActors.push_back(actor);
	else
	  dynamicActors.push_back(actor);

#ifdef DEBUG_OUTPUT
	std::cout<<
	    "Created Actor: " << name <<
	    ", shapeID: " << shapeID <<
	    ", typeID:" << typeID <<
	    std::endl;
#endif

	return actor;
}


void Simulation::update(float d_t) {
  foreach(Actor* a, dynamicActors) {
    foreach(Actor* b, staticActors) {
      Actor* aTemp = a;
      Actor* bTemp = b;

      // sortActorsByTypeID(aTemp, bTemp);

      InteractionHandlerID id
      = InteractionHandlerID(aTemp->getTypeID(), bTemp->getTypeID());
      // interactionHandlers[];
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

void Simulation::sortActorsByActorID(Actor* a, Actor* b) {
  if (a->getActorID() < b->getActorID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortActorsTypeID(Actor* a, Actor* b) {
  if (a->getTypeID() < b->getTypeID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortActorsByShapeID(Actor* a, Actor* b) {
  if (a->getShapeID() < b->getShapeID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortInt(int* a, int* b) {
  if (*a < *b) return;
  int* c = a;
  a = b;
  b = c;
}

