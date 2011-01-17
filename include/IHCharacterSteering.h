/*
 * LevelState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: deine mudda
 *  Copyright 2010 The Collage Project
 */

#include "InteractionHandler.h"

class IHCharacterSteering : public InteractionHandler {
  public:
    IHCharacterSteering();
    void inside(Interaction* interaction, float d_t);

  private:
    
};
