/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTIONHANDLER_H_
#define INTERACTIONHANDLER_H_

#include "Interaction.h"
#include "ActorFactory.h"

#include "Character.h"
#include "Terrain.h"

template <class T1, class T2>
class InteractionHandler : AbstractInteractionHandler {
 public:

  InteractionHandler() {}

  virtual ~InteractionHandler() {
    deleteAllInteractions();
  }

  std::vector<std::vector<AbstractInteraction* > > allInteractions;
  std::list<AbstractInteraction* > currentInteractions;

  void syncInteractions(
      ActorFactory<T1>* factory1,
      ActorFactory<T2>* factory2) {
    deleteAllInteractions();
    std::vector<T1*>::iterator it1 = factory1->actors.begin();
  }

  void deleteAllInteractions() {
    std::vector<std::vector<AbstractInteraction* > >::iterator it1;
    it1 = allInteractions.begin();
    for (; it1 != allInteractions.end(); it1++) {
      std::vector<AbstractInteraction*>::iterator it2;
      it2 = it1->begin();
      for (; it2 != it1->end(); it2++) {
        delete *it2;
      }
      it1->clear();
    }
    allInteractions.clear();
  }

//  void pushBackInteraction(T1* actor) {
//    printf("Actor A added.");
//  }
//
//  void pushBackInteraction(T2* actor) {
//    printf("Actor B added.");
//  }

 private:
};

#endif /* INTERACTIONHANDLER_H_ */
