/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "Simulation.h"
#include <QtCore>
#include <sstream>

Simulation::Simulation(Ogre::SceneNode *rootSceneNode) {
	this.rootSceneNode = rootSceneNode;
	currentActorID = 0;

	int a = 0;
	int b = 1;
	sortInt(&a, &b);
	interactionHandlers[InteractionHandlerID(a, b)] = new InteractionHandler();
}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
}

Actor* Simulation::createActor(
    std::string actorType,
    std::string collisionShape,
    Ogre::Vector3 position,
    bool isStatic,
    float orientation,
    float scale){
  int actorID = generateActorID();
  std::stringstream name;
  name << actorID;
	Ogre::SceneNode *sceneNode = rootSceneNode->createChildSceneNode(name, position);

	int shapeID = 0;
	switch(collisionShape){
	case "circle":
	  shapeID = 0;
	  break;
	case "box":
	  shapeID = 1;
	  break;
	}

  int typeID = 0;
  switch(actorType){
  case "item":
    shapeID = 0;
    break;
  case "character":
    shapeID = 1;
    break;
  case "terrain":
    shapeID = 1;
    break;
  }

	Actor *actor = new Actor(actorID, typeID, shapeID, sceneNode);

	if(isStatic) staticActors.push_back(actor);
	else dynamicActors.push_back(actor);

	return actor;
}

void Simulation::update(float d_t){
//	foreach(Actor *dyn, mActorListDynamic){
//		dyn->update(d_t);
//	}
//	foreach(Actor *dyn, mActorListDynamic){
//		foreach(Actor *sta, mActorListStatic){
//
//		}
//	}
  foreach(Actor* a, dynamicActors){
    foreach(Actor* b, staticActors){
      Actor* aTemp = a;
      Actor* bTemp = b;
      sortArctorsByTypeID(aTemp, bTemp);
      InteractionHandlerID id = InteractionHandlerID(aTemp->getTypeID(), bTemp->getTypeID());
      //interactionHandlers[];
    }
  }
}

int Simulation::generateActorID(){
  int ret = currentActorID;
  currentActorID++;
  return ret;
}

int Simulation::generateInteractionTypeID(){
  int ret = currentInteractionTypeID;
  currentInteractionTypeID++;
  return ret;
}

void Simulation::sortArctorsByActorID(Actor* a, Actor* b){
  if(a->getActorID() < b->getActorID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortArctorsTypeID(Actor* a, Actor* b){
  if(a->getTypeID() < b->getTypeID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortArctorsByShapeID(Actor* a, Actor* b){
  if(a->getShapeID() < b->getShapeID()) return;
  Actor* c = a;
  a = b;
  b = c;
}

void Simulation::sortInt(int* a, int* b){
  if(*a < *b)return;
  int* c = a;
  a = b;
  b = c;
}
