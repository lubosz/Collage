/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"

Character::Character() {
}

Character::~Character() {
}

void Character::print() {
  printf("character");
}

template<> void InteractionHandler<Character, Terrain>::interact(
    Interaction<Character, Terrain>* interaction) {
  interaction->a->print();
  interaction->b->print();
  interaction->ok = false;
  interaction->test = 3;
  std::cout << interaction->test << std::endl;
}
