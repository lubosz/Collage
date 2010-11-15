/*
 * GameState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "GameState.h"

// using namespace Ogre;

GameState::GameState() {
	m_MoveSpeed = 0.1f;
	m_RotateSpeed = 0.3f;

	m_bLMouseDown = false;
	m_bRMouseDown = false;
	m_bQuit = false;
	m_bSettingsMode = false;

	m_pCurrentObject = 0;
	m_pDetailsPanel = 0;
}

void GameState::enter() {
	System::Instance().logMessage(
			"Entering GameState...");

	m_pSceneMgr
			= RenderEngine::Instance().m_pRoot->createSceneManager(
					Ogre::ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pRSQ = m_pSceneMgr->createRayQuery(Ogre::Ray());
	m_pRSQ->setQueryMask(OGRE_HEAD_MASK);

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(5, 60, 60));
	m_pCamera->lookAt(Vector3(5, 20, 0));
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

bool GameState::pause() {
	System::Instance().logMessage("Pausing GameState...");

	return true;
}

void GameState::resume() {
	System::Instance().logMessage(
			"Resuming GameState...");

	buildGUI();

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

void GameState::exit() {
	System::Instance().logMessage("Leaving GameState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	m_pSceneMgr->destroyQuery(m_pRSQ);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);
}

void GameState::createScene() {
	m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

	DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
	pDotSceneLoader->parseDotScene("CubeScene.xml", "General", m_pSceneMgr,
	//pDotSceneLoader->parseDotScene("Bulldozer.scene", "General", m_pSceneMgr,
			m_pSceneMgr->getRootSceneNode());
	delete pDotSceneLoader;

	m_pSceneMgr->getEntity("Cube01")->setQueryFlags(CUBE_MASK);
	m_pSceneMgr->getEntity("Cube02")->setQueryFlags(CUBE_MASK);
	m_pSceneMgr->getEntity("Cube03")->setQueryFlags(CUBE_MASK);

	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
	m_pOgreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);
	m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
			"CubeNode");
	m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
	m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

	m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
	m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (m_bSettingsMode == true) {
		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S)) {
			OgreBites::SelectMenu
					* pMenu =
							(OgreBites::SelectMenu*)
							UserInterface::Instance().m_pTrayMgr->getWidget(
									"ChatModeSelMenu");
			if (pMenu->getSelectionIndex() + 1 < static_cast<int>(pMenu->getNumItems()))
				pMenu->selectItem(pMenu->getSelectionIndex() + 1);
		}

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W)) {
			OgreBites::SelectMenu
					* pMenu =
							(OgreBites::SelectMenu*)
							UserInterface::Instance().m_pTrayMgr->getWidget(
									"ChatModeSelMenu");
			if (pMenu->getSelectionIndex() - 1 >= 0)
				pMenu->selectItem(pMenu->getSelectionIndex() - 1);
		}
	}

	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		pushAppState(findByName("PauseState"));
		return true;
	}

	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_I)) {
		if (m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE) {
			UserInterface::Instance().m_pTrayMgr->moveWidgetToTray(
					m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
			m_pDetailsPanel->show();
		} else {
			UserInterface::Instance().m_pTrayMgr->removeWidgetFromTray(
					m_pDetailsPanel);
			m_pDetailsPanel->hide();
		}
	}

	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_TAB)) {
		m_bSettingsMode = !m_bSettingsMode;
		return true;
	}

	if (m_bSettingsMode
			&& Input::Instance().m_pKeyboard->isKeyDown(
					OIS::KC_RETURN)
			|| Input::Instance().m_pKeyboard->isKeyDown(
					OIS::KC_NUMPADENTER)) {
	}

	if (!m_bSettingsMode || (m_bSettingsMode
			&& !Input::Instance().m_pKeyboard->isKeyDown(
					OIS::KC_O)))
		Input::Instance().keyPressed(keyEventRef);

	return true;
}

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyPressed(keyEventRef);
	return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;

	if (m_bRMouseDown) {
		m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
		m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	}

	return true;
}

bool GameState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;

	if (id == OIS::MB_Left) {
		onLeftPressed(evt);
		m_bLMouseDown = true;
	} else if (id == OIS::MB_Right) {
		m_bRMouseDown = true;
	}

	return true;
}

bool GameState::mouseReleased(
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

void GameState::onLeftPressed(const OIS::MouseEvent &evt) {
	if (m_pCurrentObject) {
		m_pCurrentObject->showBoundingBox(false);
		m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMat);
	}

	Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(
			Input::Instance().m_pMouse->getMouseState().X.abs
					/ static_cast<float>(evt.state.width),
			Input::Instance().m_pMouse->getMouseState().Y.abs
					/ static_cast<float>(evt.state.height));
	m_pRSQ->setRay(mouseRay);
	m_pRSQ->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
	Ogre::RaySceneQueryResult::iterator itr;

	for (itr = result.begin(); itr != result.end(); itr++) {
		if (itr->movable) {
			System::Instance().logMessage(
					"MovableName: " + itr->movable->getName());
			m_pCurrentObject
					= m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
			System::Instance().logMessage("ObjName "
					+ m_pCurrentObject->getName());
			m_pCurrentObject->showBoundingBox(true);
			m_pCurrentEntity = m_pSceneMgr->getEntity(itr->movable->getName());
			m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMatHigh);
			break;
		}
	}
}

void GameState::moveCamera() {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCamera->moveRelative(m_TranslateVector);
	m_pCamera->moveRelative(m_TranslateVector / 10);
}

void GameState::getInput() {
	if (m_bSettingsMode == false) {
		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
			m_TranslateVector.x = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
			m_TranslateVector.x = m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W))
			m_TranslateVector.z = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
			m_TranslateVector.z = m_MoveScale;
	}
}

void GameState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		popAppState();
		return;
	}

	if (!UserInterface::Instance().m_pTrayMgr->isDialogVisible()) {
		if (m_pDetailsPanel->isVisible()) {
			m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(
					m_pCamera->getDerivedPosition().x));
			m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(
					m_pCamera->getDerivedPosition().y));
			m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(
					m_pCamera->getDerivedPosition().z));
			m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(
					m_pCamera->getDerivedOrientation().w));
			m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(
					m_pCamera->getDerivedOrientation().x));
			m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(
					m_pCamera->getDerivedOrientation().y));
			m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(
					m_pCamera->getDerivedOrientation().z));
			if (m_bSettingsMode)
				m_pDetailsPanel->setParamValue(7, "Buffered Input");
			else
				m_pDetailsPanel->setParamValue(7, "Un-Buffered Input");
		}
	}

	m_MoveScale = m_MoveSpeed * timeSinceLastFrame;
	m_RotScale = m_RotateSpeed * timeSinceLastFrame;

	m_TranslateVector = Vector3::ZERO;

	getInput();
	moveCamera();
}

void GameState::buildGUI() {
	UserInterface::Instance().m_pTrayMgr->showFrameStats(
			OgreBites::TL_BOTTOMLEFT);
	UserInterface::Instance().m_pTrayMgr->showLogo(
			OgreBites::TL_BOTTOMRIGHT);
	UserInterface::Instance().m_pTrayMgr->createLabel(
			OgreBites::TL_TOP, "GameLbl", "Game mode", 250);
	UserInterface::Instance().m_pTrayMgr->showCursor();

	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("Mode");

	m_pDetailsPanel
			= UserInterface::Instance().m_pTrayMgr->createParamsPanel(
					OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
	m_pDetailsPanel->show();

	Ogre::String
			infoText =
					"[TAB] - Switch input mode\n\n \
					[W] - Forward / Mode up\n \
					[S] - Backwards/ Mode down\n \
					[A] - Left\n";
	infoText.append(
			"[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
	infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
	UserInterface::Instance().m_pTrayMgr->createTextBox(
			OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);

	Ogre::StringVector chatModes;
	chatModes.push_back("Solid mode");
	chatModes.push_back("Wireframe mode");
	chatModes.push_back("Point mode");
	UserInterface::Instance().m_pTrayMgr->createLongSelectMenu(
			OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3,
			chatModes);
}

void GameState::itemSelected(OgreBites::SelectMenu* menu) {
  switch (menu->getSelectionIndex()) {
    case 0:
      m_pCamera->setPolygonMode(Ogre::PM_SOLID);
		break;
    case 1:
		m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		break;
    case 2:
		m_pCamera->setPolygonMode(Ogre::PM_POINTS);
		break;
	}
}
