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
  InteractionID id(a->getInteractionTypeID(), b->getInteractionTypeID());
  found = interactions.find(id);
  if (found == interactions.end()) {
    Interaction* interaction = new Interaction(a, b);
    interactions[id] = interaction;
    enter(interaction);
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
#ifdef DEBUG_OUTPUT
  std::cout<<
      "Interaction: " << "inside" <<
      ", A" << interaction->getA()->getActorID() <<
      ", A" << interaction->getB()->getActorID() <<
      std::endl;
#endif
}

void InteractionHandler::enter(Interaction* interaction) {
#ifdef DEBUG_OUTPUT
  std::cout<<
      "Interaction: " << "enter" <<
      ", Actor A: " << interaction->getA()->getActorID() <<
      ", Actor B:" << interaction->getB()->getActorID() <<
      std::endl;
#endif
}

void InteractionHandler::leave(Interaction* interaction) {
#ifdef DEBUG_OUTPUT
  std::cout<<
      "Interaction: " << "leave" <<
      ", Actor A: " << interaction->getA()->getActorID() <<
      ", Actor B:" << interaction->getB()->getActorID() <<
      std::endl;
#endif
}
