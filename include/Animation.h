/*
 * Animation.h
 *
 *  Created on: Apr 21, 2011
 *      Author: bmonkey
 *  Copyright 2011 The Collage Project
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Singleton.h"
#include <string>
#include <vector>
#include <OGRE/Ogre.h>

using std::string;
using std::vector;

class Animation : public Singleton<Animation> {
 public:
  vector<Ogre::AnimationState*> animationStates;
  bool isactive;
  Animation();
  virtual ~Animation();
  void activate(Ogre::SceneManager *sceneManager, string entityName);
  void update(double timeSinceLastFrame);
  void activate();
  void deactivate();
};

#endif /* ANIMATION_H_ */
