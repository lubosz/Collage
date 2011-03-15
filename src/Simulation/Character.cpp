/*
 * Character.cpp
 *
 *  Created on: 24.01.2011
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "Character.h"
#include "InteractionHandler.h"

#include "Terrain.h"

Character::Character() {
}

Character::~Character() {
}

void Character::print() {
  printf("character");
}

template<> void InteractionHandler<Character, Terrain>::interact(
    Character *c, Terrain *t) {
  c->print();
  t->print();
  c->getPosition();
}
