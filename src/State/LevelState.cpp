/*
 * GameState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include <OGRE/OgreTextureManager.h>

#include "LevelState.h"
#include "System.h"



LevelState::LevelState() {
	m_bLMouseDown = false;
	m_bRMouseDown = false;
	m_bQuit = false;

	connect(&genman, SIGNAL(levelGenerated(Level*)),
			this, SLOT(levelGenerated(Level*)));
}

void LevelState::levelGenerated(Level *level) {
	System::Instance().logMessage("Received level from generator...");
}

void LevelState::enter() {
	System::Instance().logMessage("Entering LevelState...");

	this->genman.requestWebpage("http://www.google.de");
	m_MoveSpeed = 0.01;

	m_pSceneMgr
		= RenderEngine::Instance().m_pRoot->createSceneManager(
				Ogre::ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(0.0, 0.0, 10));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(5);

	m_pCamera->setAspectRatio(
			Real(
				RenderEngine::Instance().m_pViewport->getActualWidth())
			/ Real(
				RenderEngine::Instance().m_pViewport->getActualHeight()));

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);

	buildGUI();

	createScene();
}

void LevelState::createScene() {
}

bool LevelState::pause() {
	System::Instance().logMessage("Pausing LevelState...");

	return true;
}

void LevelState::resume() {
	System::Instance().logMessage("Resuming LevelState...");

	buildGUI();

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

void LevelState::exit() {
	System::Instance().logMessage("Leaving LevelState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);
}

bool LevelState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		pushAppState(findByName("PauseState"));
		return true;
	}
	return true;
}

bool LevelState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyPressed(keyEventRef);
	return true;
}

	bool LevelState::mouseMoved(const OIS::MouseEvent &evt) {
		if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
			return true;

		if (m_bRMouseDown) {
			m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
			m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
		}

		return true;
	}

bool LevelState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
		) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;

	if (id == OIS::MB_Left) {
		m_bLMouseDown = true;
	} else if (id == OIS::MB_Right) {
		m_bRMouseDown = true;
	}

	return true;
}

bool LevelState::mouseReleased(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
		) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;

	if (id == OIS::MB_Left) {
		m_bLMouseDown = false;
	} else if (id == OIS::MB_Right) {
		m_bRMouseDown = false;
	}

	return true;
}

	void LevelState::getInput() {
		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
			m_TranslateVector.y = m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_DOWN))
			m_TranslateVector.y = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT))
			m_TranslateVector.x = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
			m_TranslateVector.x = m_MoveScale;
	}

void LevelState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		popAppState();
		return;
	}

	m_TranslateVector = Ogre::Vector3::ZERO;
	m_MoveScale = m_MoveSpeed * timeSinceLastFrame;
	getInput();
	m_pCamera->move(m_TranslateVector);
}

void LevelState::buildGUI() {
	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();
}

void LevelState::buttonHit(OgreBites::Button* button) {
}
