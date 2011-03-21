/*
 *  Copyright 2011 The Collage Project
 */

#include "InteractionCharacterTerrain.h"

void Interaction<Character, Terrain>::init() {
  first->print();
  second->print();
  test = 2;
  std::cout << test << std::endl;
  test = 10;
}

void Interaction<Character, Terrain>::interact() {
  first->print();
  second->print();
  test++;
  std::cout << test << std::endl;
  //  If !character_moveAlongPathSteering && Collision
  //    estimate u-coord
  //    set character_moveAlongPathSteering(u-coord)
}
