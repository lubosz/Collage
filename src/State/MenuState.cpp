/*
 * MenuState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "MenuState.h"

// using namespace Ogre;

MenuState::MenuState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void MenuState::enter() {
	System::Instance().logMessage("Entering MenuState...");

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

	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->showCursor();
	UserInterface::Instance().m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MenuLbl", "Menu mode", 250);
	UserInterface::Instance().m_pTrayMgr->createDecorWidget(OgreBites::TL_CENTER,"splashImage", "SdkTrays/Splash");
	UserInterface::Instance().m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Start Game", 250);
	UserInterface::Instance().m_pTrayMgr->createButton(OgreBites::TL_CENTER, "DeleteC", "Delete C:\\", 250);
	UserInterface::Instance().m_pTrayMgr->createButton(OgreBites::TL_CENTER, "Settings", "Settings", 250);
	UserInterface::Instance().m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Collage", 250);



	createScene();
}

void MenuState::createScene() {
}

void MenuState::exit() {
	System::Instance().logMessage("Leaving MenuState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);

	UserInterface::Instance().m_pTrayMgr->clearAllTrays();
	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->setListener(0);
}

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		m_bQuit = true;
		return true;
	}

	Input::Instance().keyPressed(keyEventRef);
	return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyReleased(keyEventRef);
	return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;
	return true;
}

bool MenuState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;
	return true;
}

bool MenuState::mouseReleased(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;
	return true;
}

void MenuState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		shutdown();
		return;
	}
}

void MenuState::buttonHit(OgreBites::Button* button) {
	if (button->getName() == "ExitBtn")
		m_bQuit = true;
	else if (button->getName() == "EnterBtn")
		popAllAndPushAppState(findByName("GameState"));
}
