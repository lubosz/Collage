/*
 * SimulationEnums.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */
#include "IHCharacterGravity.h"

enum CollisionShape {
  CS_BOX = 0,
  CS_CIRCLE = 1,
  CS_GLOBAL = 2
};

enum InteractionType {
  IT_CHARACTER = 0,
  IT_ITEM = 1,
  IT_TERRAIN = 2,
  IT_GRAVITY = 3
};

