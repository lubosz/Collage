/*
 * MenuState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "LoadingState.h"
#include "FrameCapture.h"
#include <QtCore>


// using namespace Ogre;

LoadingState::LoadingState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void LoadingState::enter() {
	System::Instance().logMessage("Entering LoadingState...");

	m_pSceneMgr
			= RenderEngine::Instance().m_pRoot->createSceneManager(
					Ogre::ST_GENERIC, "MenuSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("MenuCam");
	m_pCamera->setPosition(Vector3(0, 25, -50));
	m_pCamera->lookAt(Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);

	m_pCamera->setAspectRatio(
			Real(
					RenderEngine::Instance().m_pViewport->getActualWidth())
					/ Real(
							RenderEngine::Instance().m_pViewport->getActualHeight()));

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);

	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();
	trayManager->createLabel(OgreBites::TL_TOP, "MenuLbl", "Menu", 250);
	trayManager->showBackdrop("SdkTrays/LoadingScreen");

	createScene();
//
}

void LoadingState::createScene() {
}

void LoadingState::exit() {
	System::Instance().logMessage("Leaving MenuState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);

	UserInterface::Instance().m_pTrayMgr->hideBackdrop();
	UserInterface::Instance().m_pTrayMgr->clearAllTrays();
	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->setListener(0);
}

bool LoadingState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		m_bQuit = true;
		return true;
	}

	Input::Instance().keyPressed(keyEventRef);
	return true;
}

bool LoadingState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyReleased(keyEventRef);
	return true;
}

bool LoadingState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;
	return true;
}

bool LoadingState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;
	return true;
}

bool LoadingState::mouseReleased(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;
	return true;
}

void LoadingState::update(double timeSinceLastFrame) {
	printf("Time: %f\n", m_FrameEvent.timeSinceLastFrame);
	// draw one frame hack
	if(m_FrameEvent.timeSinceLastFrame != 0)
		popAllAndPushAppState(findByName("WallPaperLoadingState"));
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		shutdown();
		return;
	}
}

void LoadingState::buttonHit(OgreBites::Button* button) {
	if (button->getName() == "ExitBtn")
		m_bQuit = true;
	else if (button->getName() == "EnterBtn")
		popAllAndPushAppState(findByName("GameState"));
}
