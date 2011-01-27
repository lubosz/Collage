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
  ActorFactory<T1> *factory1;
  ActorFactory<T2> *factory2;
  std::vector<std::vector<Interaction<T1, T2>* > > allInteractions;
  std::list<Interaction<T1, T2>* > currentInteractions;

  InteractionHandler(ActorFactory<T1>* factory1, ActorFactory<T2>* factory2) {
    this->factory1 = factory1;
    this->factory2 = factory2;
    factory1->addInteractionHandler(this);
    factory2->addInteractionHandler(this);
    createAllInteractions(factory1, factory2);
  }

  virtual ~InteractionHandler() {
    deleteAllInteractions();
  }

  void createAllInteractions(
      ActorFactory<T1>* factory1,
      ActorFactory<T2>* factory2) {
    deleteAllInteractions();
    for (int i = 0; i < factory1->actors.size(); i++) {
      allInteractions.push_back(std::vector<Interaction<T1, T2>* >());
      for (int j = 0; j < factory2->actors.size(); j++) {
        allInteractions[i].push_back(
            new Interaction<T1, T2>(
                factory1->actors[i],
                factory2->actors[j]));
      }
    }
  }

  void deleteAllInteractions() {
    for (int i = 0; i < allInteractions.size(); i++) {
      for (int j = 0; j < allInteractions[i].size(); j++) {
        delete allInteractions[i][j];
      }
    }
    allInteractions.clear();
  }



//  void pushBackInteraction(T2* actor) {
//    printf("Actor B added.");
//  }

  void interact(Character* c, Terrain* t) {
  }


  void print() {
    printf("IH(");
    for (int i = 0; i < allInteractions.size(); i++) {
      for (int j = 0; j < allInteractions[i].size(); j++) {
        allInteractions[i][j]->print();
        printf("[%i,%i])", i, j);
      }
    }
    printf(")");
  }

 private:
};

#endif /* INTERACTIONHANDLER_H_ */
