/*
 * AppStateManager.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include <OGRE/Ogre.h>

#include "AppStateListener.h"
#include "Singleton.h"

class AppStateManager : public AppStateListener,
						public Singleton<AppStateManager> {
 public:
	typedef struct {
		Ogre::String name;
		AppState* state;
	} state_info;

	AppStateManager();
	~AppStateManager();

	void manageAppState(Ogre::String stateName, AppState* state);

	AppState* findByName(Ogre::String stateName);

	void start(std::string _state);
	void changeAppState(AppState* state);
	bool pushAppState(AppState* state);
	void popAppState(void);
	void shutdown(void);
	void popAllAndPushAppState(AppState* state);

 protected:
	void init(AppState *state);

	std::vector<AppState*> m_ActiveStateStack;
	std::vector<state_info> m_States;
	bool m_bShutdown;
};

