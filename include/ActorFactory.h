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
  virtual void pushBackInteraction(Actor* actor) {
    printf("Implementation not found.");
  }
};


class AbstractActorFactory {};


template <class T> class ActorFactory {
 public:
  ActorFactory() {}
  virtual ~ActorFactory() {}

  void addInteractionHandler(AbstractInteractionHandler* handler);

  T* createActor() {
    T* a = new T();
    actors.push_back(a);
    foreach(AbstractInteractionHandler* i, interactionHandlers) {
      i->pushBackInteraction(a);
    }
    return a;
  }

  std::vector<T*> actors;
  std::vector<AbstractInteractionHandler*> interactionHandlers;
};

#endif /* ACTORFACTORY_H_ */
