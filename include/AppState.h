/*
 * AppState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppStateListener.h"

#include "RenderEngine.h"
#include "System.h"
#include "UserInterface.h"


using Ogre::Vector3;
using Ogre::Real;
using Ogre::Degree;

//class AppStateListener;

class AppState
:
	public OIS::KeyListener,
	public OIS::MouseListener,
	public OgreBites::SdkTrayListener {
 public:
	static void create(AppStateListener* parent, const Ogre::String name) {}

	void destroy() {delete this;}

	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual bool pause() {return true;}
	virtual void resume() {}
	virtual void update(double timeSinceLastFrame) = 0;

 protected:
	AppState() {};

	AppState* findByName(Ogre::String stateName) {
		return m_pParent->findByName(stateName);
	}
	void changeAppState(AppState* state) {
		m_pParent->changeAppState(state);
	}
	bool pushAppState(AppState* state) {
		return m_pParent->pushAppState(state);
	}
	void popAppState() {
		m_pParent->popAppState();
	}
	void shutdown() {
		m_pParent->shutdown();
	}
	void popAllAndPushAppState(AppState* state) {
		m_pParent->popAllAndPushAppState(state);
	}

	AppStateListener* m_pParent;

	Ogre::Camera* m_pCamera;
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::FrameEvent m_FrameEvent;
};

#define DECLARE_APPSTATE_CLASS(T) \
static void create(AppStateListener* parent, const Ogre::String name) \
{ \
	T* myAppState = new T(); \
	myAppState->m_pParent = parent; \
	parent->manageAppState(name, myAppState); \
}

