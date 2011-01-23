/*
 *  Copyright 2010 The Collage Project
 */
#include "InteractionHandler.h"
#include <QtCore>

InteractionHandler::InteractionHandler() {
}

InteractionHandler::~InteractionHandler() {
}

Interaction* InteractionHandler::createInteraction(Actor* a, Actor* b) {
  InteractionType TypeA = a->getInteractionType();
  InteractionType TypeB = b->getInteractionType();
  switch (TypeA | TypeB) {
//  case (IT_CHARACTER | IT_TERRAIN):
//      return new InteractionCharacterTerrain();
//  case (IT_CHARACTER | IT_ENEMY):
//      return new InteractionCharacterEnemy();
  default:
    return new Interaction(a, b);
  }
}

void InteractionHandler::interact(Actor* a, Actor* b, float d_t) {
  std::map<InteractionID, Interaction*>::iterator found;
  InteractionID id(a->getActorID(), b->getActorID());
  found = interactions.find(id);
  if (found == interactions.end()) {
    Interaction* interaction = createInteraction(a, b);
// TODO(Gerrit): support multiple interaction types.
    interactions[id] = interaction;
    interaction->ok = true;
    interaction->enter();
  } else {
    found->second->ok = true;
    found->second->inside(d_t);
  }
}

void InteractionHandler::cleanup() {
  std::map<InteractionID, Interaction*>::iterator it = interactions.begin();
  for (; it != interactions.end(); it++) {
     if (!it->second->ok) {
       it->second->leave();
       interactions.erase(it);
     } else {
       it->second->ok = false;
     }
  }
}
