/*
 * SimulationEnums.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */
#include "IHCharacterGravity.h"
#include "IHCharacterSteering.h"
#include "IHCharacterTerrain.h"

enum InteractionType {
  IT_CHARACTER = 0,
  IT_ITEM,
  IT_DOOR,
  IT_TERRAIN,
  IT_GRAVITY,
  IT_STEERING
};

