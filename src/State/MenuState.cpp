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

	OgreBites::SdkTrayManager* trayManager = UserInterface::Instance().m_pTrayMgr;
	trayManager->destroyAllWidgets();

	trayManager->createButton(
	    OgreBites::TL_BOTTOMLEFT, "BrowserDemo", "Browser", 250);
	trayManager->createButton(
	    OgreBites::TL_BOTTOMLEFT, "OgreDemo", "Ogre Scene", 250);
	trayManager->createButton(
		OgreBites::TL_BOTTOMLEFT, "SimulationDemo", "SimulationDemo", 250);
	trayManager->createButton(
		OgreBites::TL_BOTTOMLEFT, "LevelState", "LevelState", 250);
	trayManager->createButton(
	    OgreBites::TL_BOTTOMLEFT, "Settings", "Settings", 250);
	trayManager->createButton(
	    OgreBites::TL_BOTTOMLEFT, "ExitBtn", "Exit Collage", 250);


	trayManager->showBackdrop("SdkTrays/Wallpaper");
	/*
	trayManager->createButton(OgreBites::TL_RIGHT, "Configure", "Configure");
	trayManager->createButton(OgreBites::TL_RIGHT, "Quit", "Quit");

	// create configuration screen button tray
	trayManager->createButton(OgreBites::TL_NONE, "Apply", "Apply Changes");
	trayManager->createButton(OgreBites::TL_NONE, "Back", "Go Back");

	// create configuration screen label and renderer menu
	trayManager->createLabel(OgreBites::TL_NONE, "ConfigLabel", "Configuration");

	OgreBites::SelectMenu* mRendererMenu = trayManager->createLongSelectMenu(OgreBites::TL_NONE, "RendererMenu", "Render System", 450, 240, 10);

	trayManager->createSeparator(OgreBites::TL_NONE, "ConfigSeparator");

	// populate render system names
	Ogre::StringVector rsNames;
	Ogre::RenderSystemList rsList = RenderEngine::Instance().m_pRoot->getAvailableRenderers();
	for (unsigned int i = 0; i < rsList.size(); i++)
	{
		rsNames.push_back(rsList[i]->getName());
	}
	mRendererMenu->setItems(rsNames);
*/
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

	UserInterface::Instance().m_pTrayMgr->hideBackdrop();
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
	else if (button->getName() == "OgreDemo")
		popAllAndPushAppState(findByName("GameState"));
	else if (button->getName() == "BrowserDemo")
		popAllAndPushAppState(findByName("WikiCubeState"));
	else if (button->getName() == "SimulationDemo")
		popAllAndPushAppState(findByName("SimulationTestState"));
	else if (button->getName() == "LevelState")
		popAllAndPushAppState(findByName("LevelState"));
}
