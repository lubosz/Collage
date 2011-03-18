/*
 *  Copyright 2010 The Collage Project
 */
#ifndef INTERACTIONHANDLER_H_
#define INTERACTIONHANDLER_H_

#include "Interaction.h"
#include "ActorFactory.h"

#include "Character.h"
#include "Terrain.h"

template<> class Interaction<Character, Terrain>
: public AbstractInteraction<Character, Terrain> {
 public:
  Interaction(Character* a, Terrain* b) {
    this->a = a;
    this->b = b;
  }
  virtual ~Interaction() {}
  float test;
};


template <class T1, class T2>
class InteractionHandler : public AbstractInteractionHandler {
 public:
  ActorFactory<T1> *factory1;
  ActorFactory<T2> *factory2;
  std::vector<std::vector<AbstractInteraction<T1, T2>* > > allInteractions;
  std::list<AbstractInteraction<T1, T2>* > currentInteractions;

  InteractionHandler(ActorFactory<T1>* factory1, ActorFactory<T2>* factory2) {
    this->factory1 = factory1;
    this->factory2 = factory2;
    factory1->addInteractionHandler(this, 0);
    factory2->addInteractionHandler(this, 1);
    createAllInteractions(factory1, factory2);
  }

  virtual ~InteractionHandler() {
    deleteAllInteractions();
  }

  void update(float d_t) {
    for (int i = 0; i < allInteractions.size(); i++) {
      for (int j = 0; j < allInteractions[i].size(); j++) {
        Interaction<T1, T2>* inter =
            static_cast<Interaction<T1, T2>* >(allInteractions[i][j]);
        interact(inter);
      }
    }
  };

  void createAllInteractions(
      ActorFactory<T1>* factory1,
      ActorFactory<T2>* factory2) {
    deleteAllInteractions();
    for (int i = 0; i < factory1->actors.size(); i++) {
      allInteractions.push_back(
              std::vector<AbstractInteraction<T1, T2>* >());
      for (int j = 0; j < factory2->actors.size(); j++) {
        allInteractions[i].push_back(
            static_cast<AbstractInteraction<T1, T2>* >(
            new Interaction<T1, T2>(
                factory1->actors[i],
                factory2->actors[j])));
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

  void pushBackInteraction() {
    if (!lastPlace) {
      allInteractions.push_back(std::vector<AbstractInteraction<T1, T2>* >());
      for (int i = 0; i < factory2->actors.size(); i++) {
        allInteractions[factory1->actors.size() - 1].push_back(
            static_cast<AbstractInteraction<T1, T2>* >(
            new Interaction<T1, T2>(
                static_cast<T1*>(lastActor),
                factory2->actors[i])));
      }
    } else {
      for (int i = 0; i < factory1->actors.size(); i++) {
        allInteractions[i].push_back(
            static_cast<AbstractInteraction<T1, T2>* >(
            new Interaction<T1, T2>(
                factory1->actors[i],
                static_cast<T2*>(lastActor))));
      }
    }
  }

  void interact(Interaction<T1, T2>* interaction);

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
