/*
 *  Copyright 2011 The Collage Project
 */

#ifndef INTERACTIONCHARACTERTERRAIN_H_
#define INTERACTIONCHARACTERTERRAIN_H_

#include "Interaction.h"
#include "Character.h"
#include "Terrain.h"

template<> class Interaction<Character, Terrain>
: public AbstractCollisionInteraction<Character, Terrain> {
 public:
  bool collisionTest(Character* first, Terrain* second);

  void onInit();

  void onEnter();

  void onLeave();

  void whileInside();

  void whileOutside();

 private:
  bool isCrossing(Ogre::Vector2 vec, Ogre::Vector2 pnt1, Ogre::Vector2 pnt2);
};
#endif /* INTERACTIONCHARACTERTERRAIN_H_ */
