/*
 * GameState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "SimulationTestState.h"

#include "System.h"
#include <OGRE/OgreTextureManager.h>

SimulationTestState::SimulationTestState() {
	m_bLMouseDown = false;
	m_bRMouseDown = false;
	m_bQuit = false;
}

void SimulationTestState::enter() {
	System::Instance().logMessage(
			"Entering SimulationTestState...");

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

bool SimulationTestState::pause() {
	System::Instance().logMessage("Pausing GameState...");

	return true;
}

void SimulationTestState::resume() {
	System::Instance().logMessage(
			"Resuming GameState...");

	buildGUI();

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

void SimulationTestState::exit() {
	System::Instance().logMessage("Leaving GameState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);
}

void SimulationTestState::createScene() {
  simulation = new Simulation(m_pSceneMgr->getRootSceneNode(), 60.0);

	m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

	simulation->createActor(
	    IT_CHARACTER, CS_CIRCLE, Ogre::Vector3(0.0, 2.0, 0.0), false);
  simulation->createActor(
      IT_TERRAIN, CS_BOX, Ogre::Vector3(0.0, -2.0, 0.0));
  simulation->createActor(
      IT_GRAVITY, CS_GLOBAL);
  simulation->attachInteractionHandler(
      IT_CHARACTER,
      IT_GRAVITY,
      new IHCharacterGravity(Ogre::Vector2(0.0, -9.81)));
}

bool SimulationTestState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		pushAppState(findByName("PauseState"));
		return true;
	}
	return true;
}

bool SimulationTestState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyPressed(keyEventRef);
	return true;
}


bool SimulationTestState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;

	if (m_bRMouseDown) {
		m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
		m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	}

	return true;
}

bool SimulationTestState::mousePressed(
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

bool SimulationTestState::mouseReleased(
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

void SimulationTestState::getInput() {
		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
			m_TranslateVector.y = m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_DOWN))
			m_TranslateVector.y = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT))
			m_TranslateVector.x = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
			m_TranslateVector.x = m_MoveScale;
}

void SimulationTestState::update(double timeSinceLastFrame) {
  simulation->update(timeSinceLastFrame);
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

void SimulationTestState::buildGUI() {
	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();
}

void SimulationTestState::buttonHit(OgreBites::Button* button) {}
