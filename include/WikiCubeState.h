/*
 * GameState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppState.h"

#include "DotSceneLoader.h"

#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreMaterialManager.h>
#include "FrameCapture.h"

class WikiCubeState : public AppState {
 public:
	WikiCubeState();

	DECLARE_APPSTATE_CLASS(WikiCubeState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera();
	void getInput();
	void buildGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void update(double timeSinceLastFrame);
	void buttonHit(OgreBites::Button* button);
	void loadBackGround(const QString & url);

 private:

	bool m_bQuit;
	Ogre::Vector3 m_TranslateVector;
	Ogre::Real m_MoveSpeed;
	Ogre::Degree m_RotateSpeed;
	float m_MoveScale;
	Ogre::Degree m_RotScale;

	bool m_bLMouseDown, m_bRMouseDown;
	Ogre::MaterialPtr cubeMat;
	FrameCapture capture;
};
