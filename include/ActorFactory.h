/*
 * ActorFactory.h
 *
 *  Created on: 23.01.2011
 *      Author: lochmann
 *  Copyright 2011 The Collage Project
 */

#ifndef ACTORFACTORY_H_
#define ACTORFACTORY_H_

#include "Actor.h"


class AbstractInteractionHandler {
 public:
  AbstractInteractionHandler() {}
  virtual ~AbstractInteractionHandler() {}
  Actor* lastActor;
  bool lastPlace;
  virtual void pushBackInteraction() = 0;
  void addInteraction(Actor* actor, bool place) {
    lastActor = actor;
    lastPlace = place;
    pushBackInteraction();
  }
};



class AbstractActorFactory {};

template <class T> class ActorFactory {
 public:
  ActorFactory() {}
  virtual ~ActorFactory() {}

  void addInteractionHandler(AbstractInteractionHandler* handler, bool place) {
    interactionHandlers.push_back(
        std::pair<AbstractInteractionHandler*, bool>(handler, place));
  }

  T* createActor() {
    T* a = new T();
    actors.push_back(a);
    for (int i = 0; i < interactionHandlers.size(); i++) {
      bool place = interactionHandlers[i].second;
      interactionHandlers[i].first->addInteraction(a, place);
    }
    return a;
  }

  std::vector<T*> actors;
  std::vector<std::pair<AbstractInteractionHandler*, bool>> interactionHandlers;
};

#endif /* ACTORFACTORY_H_ */
