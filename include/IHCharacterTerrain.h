/*
 * IHCharacterTerrain.h
 *
 *  Created on: 18.01.2011
 *      Author: lochmann
 *  Copyright 2011 The Collage Project
 */

#ifndef IHCHARACTERTERRAIN_H_
#define IHCHARACTERTERRAIN_H_

#include "Input.h"
#include "InteractionHandler.h"

class IHCharacterTerrain : public InteractionHandler {
 public:
  IHCharacterTerrain();
  virtual ~IHCharacterTerrain();
//
//  void enter(Interaction* interaction);
//  void inside(Interaction* interaction, float d_t);
//  void leave(Interaction* interaction);
};

#endif /* IHCHARACTERTERRAIN_H_ */
