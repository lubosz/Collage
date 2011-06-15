/*
 *  Copyright 2011 The Collage Project
 */

#include "Input.h"
#include "CollisionHandler.h"
#include "InteractionCharacterDoor.h"
#include "AppState.h"
#include "LevelState.h"
#include "AppStateManager.h"

bool Interaction<Character, Door>::collisionTest() {
  Ogre::Vector2 collisionNormal = Ogre::Vector2::ZERO;
  if (CollisionHandler::collisionTestAABB(
      &first->collisionShape,
      &second->collisionShape,
      &first->moveVector,
      &first->possibleMoveRatio,
      &collisionNormal)) {
    return true;
  }
  return false;
}

void Interaction<Character, Door>::onInit() {}

void Interaction<Character, Door>::onEnter() {}

void Interaction<Character, Door>::onLeave() {}

void Interaction<Character, Door>::whileInside(float d_t) {
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_SPACE)) {
	qDebug() << "Standing in front of door!";
	LevelState *levelState =
	    static_cast<LevelState*>(
	        AppStateManager::Instance().findByName("LevelState"));
	levelState->url = QString("wikidump4/") + second->url;
    AppStateManager::Instance().popAllAndPushAppState(levelState);
  }
}

void Interaction<Character, Door>::whileOutside(float d_t) {}
