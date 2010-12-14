/*
 * GameState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "WikiCubeState.h"
#include "FrameCapture.h"

// using namespace Ogre;

WikiCubeState::WikiCubeState() {
//	m_MoveSpeed = 0.1f;
//	m_RotateSpeed = 0.3f;
//
//	m_bLMouseDown = false;
//	m_bRMouseDown = false;
	m_bQuit = false;
//	m_bSettingsMode = false;
//
//	m_pCurrentObject = 0;
//	m_pDetailsPanel = 0;
}

void WikiCubeState::enter() {
	System::Instance().logMessage(
			"Entering GameState...");

	m_pSceneMgr
			= RenderEngine::Instance().m_pRoot->createSceneManager(
					Ogre::ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

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

bool WikiCubeState::pause() {
	System::Instance().logMessage("Pausing GameState...");

	return true;
}

void WikiCubeState::resume() {
	System::Instance().logMessage(
			"Resuming GameState...");

	buildGUI();

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

void WikiCubeState::exit() {
	System::Instance().logMessage("Leaving GameState...");

	m_pSceneMgr->destroyCamera(m_pCamera);
	if (m_pSceneMgr)
		RenderEngine::Instance().m_pRoot->destroySceneManager(
				m_pSceneMgr);
}

void WikiCubeState::createScene() {
	System::Instance().logMessage("Rendering Wiki image...");
	FrameCapture capture;
	capture.saveWebRender(QUrl("http://en.wikipedia.org/wiki/Special:Random") , "../Media/Textures/wall.png");

	m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);


	;
	Ogre::MaterialPtr cubeMat;
	Ogre::Entity * cubeEntity;
	Ogre::SceneNode* cubeNode;

	cubeEntity = m_pSceneMgr->createEntity("Cube", "Cube.mesh");

	cubeNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
	cubeNode->attachObject(cubeEntity);
//	cubeNode->setPosition(Vector3(0, 0, -25));

	cubeMat = cubeEntity->getSubEntity(1)->getMaterial();

}

bool WikiCubeState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
		pushAppState(findByName("PauseState"));
		return true;
	}
	return true;
}

bool WikiCubeState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	Input::Instance().keyPressed(keyEventRef);
	return true;
}

bool WikiCubeState::mouseMoved(const OIS::MouseEvent &evt) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
		return true;

	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));

	return true;
}

bool WikiCubeState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
		return true;

	return true;
}

bool WikiCubeState::mouseReleased(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
		return true;

	return true;
}

void WikiCubeState::moveCamera() {
	if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCamera->moveRelative(m_TranslateVector);
		m_pCamera->moveRelative(m_TranslateVector / 10);
}

void WikiCubeState::getInput() {
		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
			m_TranslateVector.x = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
			m_TranslateVector.x = m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W))
			m_TranslateVector.z = -m_MoveScale;

		if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
			m_TranslateVector.z = m_MoveScale;
}

void WikiCubeState::update(double timeSinceLastFrame) {
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
			m_FrameEvent);

	if (m_bQuit == true) {
		popAppState();
		return;
	}

	m_MoveScale = m_MoveSpeed * timeSinceLastFrame;
	m_RotScale = m_RotateSpeed * timeSinceLastFrame;

	m_TranslateVector = Vector3::ZERO;

	getInput();
	moveCamera();
}

void WikiCubeState::buildGUI() {

}

