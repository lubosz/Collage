/*
 * AppState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include <QObject>

#include "AppStateListener.h"

#include "RenderEngine.h"
#include "System.h"
#include "UserInterface.h"

#include "AppStateManager.h"


using Ogre::Vector3;
using Ogre::Real;
using Ogre::Degree;

class AppState
:
	public QObject,
	public OIS::KeyListener,
	public OIS::MouseListener,
	public OgreBites::SdkTrayListener {
	Q_OBJECT
 public:
	// static void create(const Ogre::String name) {}

	void destroy() {delete this;}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause() {return true;}
	virtual void resume() {}
	virtual void update(double timeSinceLastFrame) = 0;

 protected:
    AppState() {}

	AppState* findByName(Ogre::String stateName) {
		return AppStateManager::Instance().findByName(stateName);
	}
	void changeAppState(AppState* state) {
		AppStateManager::Instance().changeAppState(state);
	}
	bool pushAppState(AppState* state) {
		return AppStateManager::Instance().pushAppState(state);
	}
	void popAppState() {
		AppStateManager::Instance().popAppState();
	}
	void shutdown() {
		AppStateManager::Instance().shutdown();
	}
	void popAllAndPushAppState(AppState* state) {
		AppStateManager::Instance().popAllAndPushAppState(state);
	}

	AppStateListener* m_pParent;

	Ogre::Camera* m_pCamera;
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::FrameEvent m_FrameEvent;
};

#define DECLARE_APPSTATE_CLASS(T) \
static void create(const Ogre::String name) \
{ \
	T* myAppState = new T(); \
	AppStateManager::Instance().manageAppState(name, myAppState); \
}

