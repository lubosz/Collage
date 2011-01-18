/*
 * LevelState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include <OGRE/OgreTextureManager.h>

#include "LevelState.h"
#include "System.h"

using std::string;

LevelState::LevelState() {
  moveSpeed = 0.5f;
  rotateSpeed = -0.5f;

  lMouseDown = false;
  rMouseDown = false;
  quit = false;

  connect(&genman, SIGNAL(levelGenerated(Level*)),
          this, SLOT(levelGenerated(Level*)));
}

void LevelState::enter() {
  Input::Instance().m_pMouse->setBuffered(false);

  // Set up Camera
  System::Instance().logMessage("Entering LevelState...");

  m_pSceneMgr = RenderEngine::Instance().m_pRoot->createSceneManager(
          Ogre::ST_GENERIC, "LevelSceneManager");
  m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

  m_pCamera = m_pSceneMgr->createCamera("GameCamera");
  m_pCamera->setPosition(Vector3(-5, 45, 100));
  m_pCamera->lookAt(Vector3(0, 0, 0));
  m_pCamera->setNearClipDistance(0.1);

  m_pCamera->setAspectRatio(Real(RenderEngine::Instance().m_pViewport->
    getActualWidth()) / Real(RenderEngine::Instance().m_pViewport->
      getActualHeight()));

  RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);


  // Set up character geometry
	Ogre::Entity *charEntity = m_pSceneMgr->createEntity("Char", "character.mesh");
	Ogre::Entity *doorEntity = m_pSceneMgr->createEntity("door.mesh");
  m_pSceneMgr->getRootSceneNode()->createChildSceneNode("Door");
  // const float rad = 90. * (3.145 / 180.);
  // charNode->rotate(Ogre::UNIT_Z, rad, Ogre::relativeTo);
	// m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

	// m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
	// m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
	// m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
	// m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);

  // Set up physics simulation with 100 Hz
  simulation = new Simulation(m_pSceneMgr->getRootSceneNode(), 100.0);

  // Character
	simulation->createActor(
	    IT_STEERING, CT_GLOBAL);
	Ogre::SceneNode *actorNode = simulation->createActor(
	    IT_CHARACTER, CT_AABB,
	    Ogre::Vector3(0.0, 2.0, 0.0), false)->getSceneNode();
      actorNode->attachObject(charEntity);
  simulation->attachInteractionHandler(
      IT_CHARACTER,
      IT_STEERING,
      new IHCharacterSteering());

  simulation->attachInteractionHandler(
      IT_CHARACTER, IT_TERRAIN, new IHCharacterTerrain());

  // Gravity
  simulation->createActor(
      IT_GRAVITY, CT_GLOBAL);
  simulation->attachInteractionHandler(
      IT_CHARACTER,
      IT_GRAVITY,
      new IHCharacterGravity(Ogre::Vector2(0.0, -9.81)));

  // Generate Level
  genman.sceneFromUrl(
//      "http://www.youtube.com/watch?v=urAyOKlgGDk",
//      "http://www.randomwebsite.com/cgi-bin/random.pl",
      "http://en.wikipedia.org/wiki/Special:Random",
//      "http://www.libpng.org/pub/png/png-rgba32.html",
//      "http://webkit.org/",
//        "http://the-space-station.com",
      m_pSceneMgr, simulation);

  // Build gui (surprise!)
  buildGUI();
}

bool LevelState::pause() {
  System::Instance().logMessage("Pausing LevelState...");

  return true;
}

void LevelState::resume() {
  System::Instance().logMessage(
      "Resuming LevelState...");

  buildGUI();

  RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
  quit = false;
}

void LevelState::exit() {
  Input::Instance().m_pMouse->setBuffered(true);
  System::Instance().logMessage("Leaving LevelState...");

  m_pSceneMgr->destroyCamera(m_pCamera);
  if (m_pSceneMgr)
    RenderEngine::Instance().m_pRoot->destroySceneManager(
        m_pSceneMgr);
}

void LevelState::levelGenerated(Level *level) {
       System::Instance().logMessage("Received level from generator...");
}

bool LevelState::keyPressed(const OIS::KeyEvent &keyEventRef) {
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
    pushAppState(findByName("PauseState"));
    return true;
  }

  return true;
}

bool LevelState::keyReleased(const OIS::KeyEvent &keyEventRef) {
  Input::Instance().keyPressed(keyEventRef);
  return true;
}

bool LevelState::mouseMoved(const OIS::MouseEvent &evt) {
  if (UserInterface::Instance().m_pTrayMgr->injectMouseMove(evt))
    return true;

  return true;
}

bool LevelState::mousePressed(
    const OIS::MouseEvent &evt,
    OIS::MouseButtonID id
  ) {
  if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
    return true;

  if (id == OIS::MB_Left) {
    lMouseDown = true;
  } else if (id == OIS::MB_Right) {
    rMouseDown = true;
  }

  return true;
}

bool LevelState::mouseReleased(
    const OIS::MouseEvent &evt,
    OIS::MouseButtonID id
  ) {
  if (UserInterface::Instance().m_pTrayMgr->injectMouseUp(evt, id))
    return true;

  if (id == OIS::MB_Left) {
    lMouseDown = false;
  } else if (id == OIS::MB_Right) {
    rMouseDown = false;
  }

  return true;
}

void LevelState::moveCamera() {
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
    m_pCamera->moveRelative(translateVector*5);
  m_pCamera->moveRelative(translateVector);
}

void LevelState::getInput() {
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
      translateVector.x = -moveSpeed;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
      translateVector.x = moveSpeed;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W))
      translateVector.z = -moveSpeed;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
      translateVector.z = moveSpeed;
}

void LevelState::update(double timeSinceLastFrame) {
  m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
  simulation->update(timeSinceLastFrame*0.000001);
  UserInterface::Instance().m_pTrayMgr->frameRenderingQueued(
      m_FrameEvent);

  if (quit == true) {
    popAppState();
    return;
  }

  moveScale = moveSpeed * timeSinceLastFrame;
  rotScale = rotateSpeed * timeSinceLastFrame;

  translateVector = Vector3::ZERO;

  getInput();
  moveCamera();

  OIS::MouseState &mutableMouseState =
      const_cast<OIS::MouseState &>(
          Input::Instance().m_pMouse->getMouseState());
  m_pCamera->yaw(Degree(mutableMouseState.X.rel * rotateSpeed));
  m_pCamera->pitch(Degree(mutableMouseState.Y.rel * rotateSpeed));
}

void LevelState::buildGUI() {
  UserInterface::Instance().m_pTrayMgr->showFrameStats(
      OgreBites::TL_BOTTOMLEFT);
  UserInterface::Instance().m_pTrayMgr->createLabel(
      OgreBites::TL_TOP, "GameLbl", "Level mode", 250);
  UserInterface::Instance().m_pTrayMgr->hideCursor();
}
