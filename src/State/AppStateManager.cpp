/*
 * AppStateManager.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include <OgreWindowEventUtilities.h>
#include <QApplication>
#include "AppStateManager.h"
#include "RenderEngine.h"
#include "System.h"
#include "Input.h"
#include "UserInterface.h"
#include "AppState.h"
#include "Animation.h"

AppStateManager::AppStateManager() {
	m_bShutdown = false;
}


AppStateManager::~AppStateManager() {
	while (!m_ActiveStateStack.empty()) {
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	while (!m_States.empty()) {
		m_States.pop_back();
	}
}

void AppStateManager::manageAppState(Ogre::String stateName, AppState* state) {
	try {
		state_info new_state_info;
		new_state_info.name = stateName;
		new_state_info.state = state;
		m_States.push_back(new_state_info);
	} catch(const std::exception& e) {
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Error while trying to manage a new AppState\n" + Ogre::String(
						e.what()), "AppStateManager.cpp (39)");
	}
}

AppState* AppStateManager::findByName(Ogre::String stateName) {
	std::vector<state_info>::iterator itr;

	for (itr = m_States.begin(); itr != m_States.end(); itr++) {
		if (itr->name == stateName)
			return itr->state;
	}

	return 0;
}

void AppStateManager::start(AppState* state) {
	changeAppState(state);

	int timeSinceLastFrame = 1;
	int startTime = 0;

	while (!m_bShutdown) {
		if (RenderEngine::Instance().m_pRenderWnd->isClosed())
			m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		Ogre::WindowEventUtilities::messagePump();
#endif
		if (RenderEngine::Instance().m_pRenderWnd->isActive()) {
		  startTime	= System::Instance().m_pTimer->getMicroseconds();
			Input::Instance().m_pKeyboard->capture();
			Input::Instance().m_pMouse->capture();

			RenderEngine::Instance().m_pRoot->renderOneFrame();

			m_ActiveStateStack.back()->update(timeSinceLastFrame);
			RenderEngine::Instance().updateOgre(timeSinceLastFrame);
			Animation::Instance().update(timeSinceLastFrame * 0.000001);

			timeSinceLastFrame
					= System::Instance().m_pTimer->getMicroseconds()
							- startTime;
		} else {
#ifdef WIN32
			 Sleep(1000);
#else
			 sleep(1);
#endif
		}
	}

	System::Instance().logMessage("Main loop quit");
}

void AppStateManager::changeAppState(AppState* state) {
	if (!m_ActiveStateStack.empty()) {
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();
}

bool AppStateManager::pushAppState(AppState* state) {
	if (!m_ActiveStateStack.empty()) {
		if (!m_ActiveStateStack.back()->pause())
			return false;
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();

	return true;
}

void AppStateManager::popAppState(void) {
	if (!m_ActiveStateStack.empty()) {
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	if (!m_ActiveStateStack.empty()) {
		init(m_ActiveStateStack.back());
		m_ActiveStateStack.back()->resume();
	} else {
		shutdown();
	}
}

void AppStateManager::popAllAndPushAppState(AppState* state) {
	while (!m_ActiveStateStack.empty()) {
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	pushAppState(state);
}

void AppStateManager::shutdown() {
	m_bShutdown = true;
}

void AppStateManager::init(AppState* state) {
	Input::Instance().m_pKeyboard->setEventCallback(state);
	Input::Instance().m_pMouse->setEventCallback(state);
	UserInterface::Instance().m_pTrayMgr->setListener(state);

	RenderEngine::Instance().m_pRenderWnd->resetStatistics();
}

void AppStateManager::pauseAppState(AppState* state) {
}
