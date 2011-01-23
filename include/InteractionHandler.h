/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTIONHANDLER_H_
#define INTERACTIONHANDLER_H_

#include "Interaction.h"

class InteractionHandler {
 public:
  InteractionHandler();
  virtual ~InteractionHandler();

  virtual Interaction* createInteraction(Actor* a, Actor* b);

  void interact(Actor* a, Actor* b, float d_t);

  void cleanup();

 private:
  typedef std::pair<int, int> InteractionID;
  std::map<InteractionID, Interaction*> interactions;
};

#endif /* INTERACTIONHANDLER_H_ */
