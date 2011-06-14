/*
 * Animation.cpp
 *
 *  Created on: Apr 21, 2011
 *      Author: bmonkey
 *  Copyright 2011 The Collage Project
 */

#include <QtCore>
#include "Animation.h"

Animation::Animation() {
}

Animation::~Animation() {
}

void Animation::activate(Ogre::SceneManager *sceneManager, string entityName) {
  Ogre::Entity * entity = sceneManager->getEntity(entityName);
  Ogre::SkeletonInstance *skeletonInstance = entity->getSkeleton();

  if (skeletonInstance) {
    for (unsigned i = 0; i < skeletonInstance->getNumAnimations(); ++i) {
      Ogre::Animation *animation = skeletonInstance->getAnimation(i);
      std::cout << "Animationname " << animation->getName();
    }
    Ogre::AnimationState* animationState = entity->getAnimationState("run");
//    Ogre::AnimationState* animationState =
//        entity->getAnimationState("NlaTrack");
    animationState->setLoop(true);
    animationState->setEnabled(true);
    animationStates.push_back(animationState);
  }
}

void Animation::update(double timeSinceLastFrame) {
  foreach(Ogre::AnimationState* animationState, animationStates) {
    animationState->addTime(timeSinceLastFrame);
  }
}
