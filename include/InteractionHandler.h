/*
 * InteractionHandler.h
 *
 *  Created on: 13.01.2011
 *      Author: lochmann
 */

#ifndef INTERACTIONHANDLER_H_
#define INTERACTIONHANDLER_H_

#include "Interaction.h"

class InteractionHandler {
public:
  InteractionHandler();
  virtual ~InteractionHandler();

  void interact(Actor* a, Actor* b, float d_t);

  void cleanup();

  virtual void inside(Interaction* interaction, float d_t);

  virtual void enter(Interaction* interaction);

  virtual void leave(Interaction* interaction);

private:
  typedef std::pair<int, int> InteractionID;
  std::map<InteractionID, Interaction*> interactions;
};

#endif /* INTERACTIONHANDLER_H_ */
