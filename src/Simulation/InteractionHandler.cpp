/*
 *  Copyright 2010 The Collage Project
 */
#include "InteractionHandler.h"
#include <QtCore>

InteractionHandler::InteractionHandler() {
}

InteractionHandler::~InteractionHandler() {
}

void InteractionHandler::interact(Actor* a, Actor* b, float d_t) {
  std::map<InteractionID, Interaction*>::iterator found;
  found = interactions.find(InteractionID(a->getTypeID(), b->getTypeID()));
  if (found == interactions.end()) {
    enter(found->second);
  } else {
    inside(found->second, d_t);
  }
}

void InteractionHandler::cleanup() {
  std::map<InteractionID, Interaction*>::iterator it = interactions.begin();
  for (; it != interactions.begin(); it++) {
     if (!it->second->getOk()) {
       leave(it->second);
       interactions.erase(it);
     }
  }
}


void InteractionHandler::inside(Interaction* interaction, float d_t) {
}

void InteractionHandler::enter(Interaction* interaction) {
}

void InteractionHandler::leave(Interaction* interaction) {
}
