/*
 * PauseState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "PauseState.h"
#include "LevelState.h"


PauseState::PauseState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void PauseState::enter() {
  Input::Instance().m_pMouse->setBuffered(true);
	System::Instance().logMessage("Entering PauseState...");

	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();
	trayManager->showCursor();
	trayManager->createButton(
	    OgreBites::TL_CENTER, "BackToGameBtn", "Resume Game", 250);
  trayManager->createButton(
      OgreBites::TL_CENTER, "RestartGameBtn", "Restart Game", 250);
	trayManager->createButton(
	    OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
//  trayManager->createButton(
//    OgreBites::TL_CENTER, "ExitBtn", "Exit Collage", 250);
	trayManager->createLabel(OgreBites::TL_TOP, "PauseLbl", "Pause", 250);
	trayManager->showBackdrop("SdkTrays/Shade");
	m_bQuit = false;
}

void PauseState::exit() {
  UserInterface::Instance().m_pTrayMgr->hideCursor();
	System::Instance().logMessage("Leaving PauseState...");

	UserInterface::Instance().m_pTrayMgr->hideBackdrop();
	UserInterface::Instance().m_pTrayMgr->clearAllTrays();
	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->setListener(0);
}

bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		m_bQuit = true;
		return true;
	}
	Input::Instance().keyPressed();
	return true;
}

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyReleased();
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
	if (button->getName() == "ExitBtn") {
		UserInterface::Instance().m_pTrayMgr->showYesNoDialog("Sure?",
				"Really leave?");
	}	else if (button->getName() == "BackToGameBtn") {
		m_bQuit = true;
	} else if (button->getName() == "RestartGameBtn") {
    LevelState* ls = reinterpret_cast<LevelState*>(findByName("LevelState"));
    ls->url = BASE_URL;
    popAllAndPushAppState(findByName("LevelState"));
  }	else if (button->getName() == "BackToMenuBtn") {
		popAllAndPushAppState(findByName("MenuState"));
  }
}

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question,
		bool yesHit) {
	if (yesHit == true)
		shutdown();
	else
		UserInterface::Instance().m_pTrayMgr->closeDialog();
}
