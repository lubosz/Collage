/*
 * LostState
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "LostState.h"


LostState::LostState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void LostState::enter() {
  Input::Instance().m_pMouse->setBuffered(true);
	System::Instance().logMessage("Entering PauseState...");

	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();
	trayManager->showCursor();
	trayManager->createDecorWidget(
	    OgreBites::TL_CENTER, "youlost", "SdkTrays/Lostthegame");
	trayManager->createButton(
	    OgreBites::TL_CENTER, "RestartGameBtn", "Restart Game", 250);
	trayManager->createButton(
	    OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
	trayManager->createLabel(OgreBites::TL_TOP, "PauseLbl", "Lost", 250);
	trayManager->showBackdrop("SdkTrays/Shade");
	m_bQuit = false;
}

void LostState::exit() {
	System::Instance().logMessage("Leaving LostState...");

	UserInterface::Instance().m_pTrayMgr->hideBackdrop();
	UserInterface::Instance().m_pTrayMgr->clearAllTrays();
	UserInterface::Instance().m_pTrayMgr->destroyAllWidgets();
	UserInterface::Instance().m_pTrayMgr->setListener(0);
}

bool LostState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		m_bQuit = true;
		return true;
	}

	Input::Instance().keyPressed(keyEventRef);

	return true;
}

bool LostState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyReleased(keyEventRef);

	return true;
}

bool LostState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;
	return true;
}

bool LostState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;
	return true;
}

bool LostState::mouseReleased(const OIS::MouseEvent &evt,
		OIS::MouseButtonID id) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;
	return true;
}

void LostState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		popAppState();
		return;
	}
}

void LostState::buttonHit(OgreBites::Button *button) {
	if (button->getName() == "ExitBtn")
		UserInterface::Instance().m_pTrayMgr->showYesNoDialog("Sure?",
				"Really leave?");
	else if (button->getName() == "RestartGameBtn")
	  popAllAndPushAppState(findByName("LevelState"));
	else if (button->getName() == "BackToMenuBtn")
		popAllAndPushAppState(findByName("MenuState"));
}

void LostState::yesNoDialogClosed(const Ogre::DisplayString& question,
		bool yesHit) {
	if (yesHit == true)
		shutdown();
	else
		UserInterface::Instance().m_pTrayMgr->closeDialog();
}
