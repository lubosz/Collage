/*
 * PauseState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "PauseState.h"


PauseState::PauseState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void PauseState::enter() {
	System::Instance().logMessage("Entering PauseState...");

	m_pSceneMgr
			= RenderEngine::Instance().m_pRoot->createSceneManager(
					Ogre::ST_GENERIC, "PauseSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("PauseCam");
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
	UserInterface::Instance().m_pTrayMgr->createButton(
			OgreBites::TL_CENTER, "BackToGameBtn", "Return to GameState", 250);
	UserInterface::Instance().m_pTrayMgr->createButton(
			OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
	UserInterface::Instance().m_pTrayMgr->createButton(
			OgreBites::TL_CENTER, "ExitBtn", "Exit Collage", 250);
	UserInterface::Instance().m_pTrayMgr->createLabel(
			OgreBites::TL_TOP, "PauseLbl", "Pause mode", 250);

	m_bQuit = false;

	createScene();
}

void PauseState::createScene() {
}


void PauseState::exit() {
	System::Instance().logMessage("Leaving PauseState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);

	UserInterface::Instance().m_pTrayMgr->clearAllTrays();
	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->setListener(0);
}

bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		m_bQuit = true;
		return true;
	}

	Input::Instance().keyPressed(keyEventRef);

	return true;
}

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyReleased(keyEventRef);

	return true;
}

bool PauseState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;
	return true;
}

bool PauseState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;
	return true;
}

bool PauseState::mouseReleased(const OIS::MouseEvent &evt,
		OIS::MouseButtonID id) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;
	return true;
}

void PauseState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		popAppState();
		return;
	}
}

void PauseState::buttonHit(OgreBites::Button *button) {
	if (button->getName() == "ExitBtn")
		UserInterface::Instance().m_pTrayMgr->showYesNoDialog("Sure?",
				"Really leave?");
	else if (button->getName() == "BackToGameBtn")
		m_bQuit = true;
	else if (button->getName() == "BackToMenuBtn")
		popAllAndPushAppState(findByName("MenuState"));
}

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question,
		bool yesHit) {
	if (yesHit == true)
		shutdown();
	else
		UserInterface::Instance().m_pTrayMgr->closeDialog();
}
