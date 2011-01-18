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
    interaction->setOk(true);
  } else {
    found->second->setOk(true);
    inside(found->second, d_t);
  }
}

void InteractionHandler::cleanup() {
  std::map<InteractionID, Interaction*>::iterator it = interactions.begin();
  for (; it != interactions.end(); it++) {
     if (!it->second->getOk()) {
       leave(it->second);
       interactions.erase(it);
     } else {
       it->second->setOk(false);
     }
  }
}


void InteractionHandler::inside(Interaction* interaction, float d_t) {
#ifdef DEBUG_OUTPUT_TRIGGERED
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
