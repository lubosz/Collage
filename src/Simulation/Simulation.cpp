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

Simulation::Simulation(Ogre::SceneNode *rootSceneNode, float frequency) {
	this->rootSceneNode = rootSceneNode;
	this->frequency = frequency;
	currentActorID = 0;
	d_t = 0.0;
}

Simulation::~Simulation() {}


Actor* Simulation::createActor(
    InteractionType interactionType,
    CollisionType collisionType,
    Ogre::Vector3 position,
    bool isStatic,
    float orientation,
    float scale) {
  int actorID = generateActorID();
  std::stringstream name;
  name << actorID;
	Ogre::SceneNode *sceneNode =
	    rootSceneNode->createChildSceneNode(name.str(), position);



	Actor *actor = new Actor(
	    actorID,
	    interactionType,
	    collisionType,
	    sceneNode);

	if(isStatic)
	  staticActors.push_back(actor);
	else
	  dynamicActors.push_back(actor);

	return actor;
}

void Simulation::attachInteractionHandler(
    InteractionType actorTypeA,
    InteractionType actorTypeB,
    InteractionHandler* interactionHandler) {
  int a = actorTypeA;
  int b = actorTypeB;
  sortInt(&a, &b);
  interactionHandlers[InteractionHandlerID(a, b)] = interactionHandler;
  interactionHandlers.find(InteractionHandlerID(a, b))->second->cleanup();
}

void Simulation::update(float secondsSinceLastFrame) {
  d_t += secondsSinceLastFrame;
  if (d_t > 1.0 / frequency) {
#ifdef DEBUG_OUTPUT_TRIGGERED
    printf("NEW FRAME, d_t: %f \n", d_t);
#endif

    foreach(Actor* a, dynamicActors) {
      foreach(Actor* b, staticActors) {
        bool collision = false;

#ifdef DEBUG_OUTPUT_TRIGGERED
        printf("collision test: A%i, A%i -> ",
            a->getActorID(),
            b->getActorID());
#endif

        // Collision Tests
        switch (b->getCollisionTypeID()) {
        case CT_GLOBAL:
          collision = true;
#ifdef DEBUG_OUTPUT_TRIGGERED
          printf("GLOBAL \n");
#endif
          break;
        case CT_AABB:
          switch (a->getCollisionTypeID()) {
          case CT_AABB:
#ifdef DEBUG_OUTPUT_TRIGGERED
            printf("AABB, AABB -> ");
#endif
            CSAABB* aCS = static_cast<CSAABB*>(a->getCollisionShape());
            CSAABB* bCS = static_cast<CSAABB*>(b->getCollisionShape());
            float aCx = a->getSceneNode()->getPosition().x;
            float bCx = b->getSceneNode()->getPosition().x;
            float aCy = a->getSceneNode()->getPosition().y;
            float bCy = b->getSceneNode()->getPosition().y;
            float aRx = aCS->aABB.x/2;
            float bRx = bCS->aABB.x/2;
            float aRy = aCS->aABB.y/2;
            float bRy = bCS->aABB.y/2;
            collision = true;
            if (abs(aCx - bCx) > (aRx + bRx)) collision = false;
            if (abs(aCy - bCy) > (aRy + bRy)) collision = false;
#ifdef DEBUG_OUTPUT_TRIGGERED
            if (collision) printf("TRUE \n");
            else
              printf("FALSE \n");
#endif
            break;
          }
          break;
        default:
#ifdef DEBUG_OUTPUT_TRIGGERED
          printf("NONE \n");
#endif
          break;
        }

        // Find type specific interaction
        if (collision) {
        Actor* aTemp = a;
        Actor* bTemp = b;
        sortActorsByInteractionTypeID(&aTemp, &bTemp);
        InteractionHandlerID id =
            InteractionHandlerID(
                aTemp->getInteractionTypeID(),
                bTemp->getInteractionTypeID());
        std::map<InteractionHandlerID, InteractionHandler*>::iterator it =
            interactionHandlers.find(id);
          if (it != interactionHandlers.end()) {
            it->second->interact(aTemp, bTemp, d_t);
#ifdef DEBUG_OUTPUT_TRIGGERED
            printf("interaction handler: %i, %i \n",
                aTemp->getInteractionTypeID(),
                bTemp->getInteractionTypeID());
          } else {
            printf("no interaction handler for: %i, %i \n",
                aTemp->getInteractionTypeID(),
                bTemp->getInteractionTypeID());
#endif
          }
        }
      }
    }

    foreach(Actor* a, dynamicActors) {
      a->update(d_t);
    }
    d_t = 0.0;
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

void Simulation::sortActorsByInteractionTypeID(Actor** a, Actor** b) {
  if ((*a)->getInteractionTypeID() < (*b)->getInteractionTypeID()) return;
  Actor* c = *a;
  *a = *b;
  *b = c;
}

void Simulation::sortActorsByCollisionTypeID(Actor** a, Actor** b) {
  if ((*a)->getCollisionTypeID() < (*b)->getCollisionTypeID()) return;
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

