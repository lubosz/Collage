/*
 * SimulationEnums.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */
#include "IHCharacterGravity.h"
#include "IHCharacterSteering.h"

enum InteractionType {
  IT_CHARACTER = 0,
  IT_ITEM = 1,
  IT_TERRAIN = 2,
  IT_GRAVITY = 3,
  IT_STEERING = 4
};

