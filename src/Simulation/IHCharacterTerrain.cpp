/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionHandler.h"
#include "Character.h"
#include "Terrain.h"

//  template<> class Interaction<Character, Terrain>
//  : public AbstractInteraction<Character, Terrain> {
//   public:
//    float test;
//  };

template<> void InteractionHandler<Character, Terrain>::interact(
    Interaction<Character, Terrain>* interaction) {
  interaction->a->print();
  interaction->b->print();
//  interaction->test = 3;
//  std::cout << interaction->test << std::endl;
}
