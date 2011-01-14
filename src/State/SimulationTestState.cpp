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

	mSimulation = new Simulation(m_pSceneMgr->getRootSceneNode());

	m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

	mSimulation->createActor("terrain", "box", Ogre::Vector3(1.0,0.0,0.0));
	mSimulation->createActor("terrain", "box", Ogre::Vector3(2.0,0.0,0.0));
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
	mSimulation->update(timeSinceLastFrame);
}

void SimulationTestState::buildGUI() {
	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();

//	trayManager->showCursor();
//	trayManager->createButton(OgreBites::TL_LEFT, "LoadWiki", "Wikipedia", 250);
//	trayManager->createButton(OgreBites::TL_LEFT, "LoadXKCD", "XKCD", 250);
//	trayManager->createButton(OgreBites::TL_LEFT, "Reddit", "Reddit", 250);
//	trayManager->createButton(OgreBites::TL_LEFT, "4chan", "4chan/b/", 250);
	//	trayManager->createButton(OgreBites::TL_LEFT, "Debug", "Debug", 250);
}

void SimulationTestState::buttonHit(OgreBites::Button* button) {

//	if (button->getName() == "LoadWiki")
//		loadBackGround("http://en.wikipedia.org/wiki/Special:Random");
//	else if (button->getName() == "LoadXKCD")
//		loadBackGround("http://dynamic.xkcd.com/random/comic/");
//	else if(button->getName() == "Debug")
//		loadBackGround("http://bstation/collage/");
//	else if(button->getName() == "Reddit")
//		loadBackGround("http://www.reddit.com/r/random/");
//	else if(button->getName() == "4chan")
//			loadBackGround("http://boards.4chan.org/b/");

}
