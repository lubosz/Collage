/*
 * AppStateListener.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include <OGRE/Ogre.h>
//#include "AppState.h"
class AppState;

class AppStateListener {
 public:
	AppStateListener() {}
	virtual ~AppStateListener() {}

	virtual void manageAppState(Ogre::String stateName, AppState* state) = 0;

	virtual AppState* findByName(Ogre::String stateName) = 0;
	virtual void changeAppState(AppState *state) = 0;
	virtual bool pushAppState(AppState* state) = 0;
	virtual void popAppState() = 0;
	virtual void pauseAppState(AppState* state) = 0;
	virtual void shutdown() = 0;
	virtual void popAllAndPushAppState(AppState* state) = 0;
};

