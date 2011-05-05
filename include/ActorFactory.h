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
  virtual void update(float d_t) = 0;
  Actor* lastActor;
  bool lastPlace;
  virtual void pushBackInteraction() = 0;
  void addInteraction(Actor* actor, bool place) {
    lastActor = actor;
    lastPlace = place;
    pushBackInteraction();
  }
//  virtual void print();
};


class AbstractActorFactory {
 public:
  AbstractActorFactory() {}
  virtual ~AbstractActorFactory() {}
  virtual void manipulate(float d_t) = 0;
  virtual void update(float d_t) = 0;
};

template <class T> class ActorFactory : public AbstractActorFactory {
 public:
  ActorFactory() {}
  ActorFactory(
      Ogre::SceneManager* sceneManager,
      std::string name) {
    this->id = 0;
    this->name = name;
    this->sceneManager = sceneManager;
  }
  virtual ~ActorFactory() {}

  std::string name;
  int id;
  Ogre::SceneManager* sceneManager;
  std::vector<T*> actors;
  std::vector<std::pair<AbstractInteractionHandler*, bool> >
    interactionHandlers;

  void addInteractionHandler(AbstractInteractionHandler* handler, bool place) {
    interactionHandlers.push_back(
        std::pair<AbstractInteractionHandler*, bool>(handler, place));
  }

  T* createActor() {
    T* a = new T();
    std::stringstream out;
    out << name << "_" << id;
    a->id = out.str();
    actors.push_back(a);
    for (int i = 0; i < interactionHandlers.size(); i++) {
      bool place = interactionHandlers[i].second;
      interactionHandlers[i].first->addInteraction(a, place);
    }
    id++;
    a->sceneManager = sceneManager;
    a->sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    a->init();
    return a;
  }

  void manipulate(float d_t) {
    for (int i = 0; i < actors.size(); i++) {
      actors[i]->manipulate(d_t);
    }
  }

  void update(float d_t) {
    for (int i = 0; i < actors.size(); i++) {
      actors[i]->update(d_t);
    }
  }
};

#endif /* ACTORFACTORY_H_ */
