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
#include "Animation.h"

using std::string;

LevelState::LevelState() {
  moveSpeed = 0.5f;
  rotateSpeed = -0.5f;

  lMouseDown = false;
  rMouseDown = false;
  quit = false;

  this->level = NULL;
}

void LevelState::enter() {
  Input::Instance().m_pMouse->setBuffered(false);

  // Set up Camera
  System::Instance().logMessage("Entering LevelState...");

  m_pSceneMgr = RenderEngine::Instance().m_pRoot->createSceneManager(
          Ogre::ST_GENERIC, "LevelSceneManager");

  LevelManager levelman(m_pSceneMgr);
  connect(&levelman, SIGNAL(levelGenerated(Level*)),
    this, SLOT(levelGenerated(Level*)));

  m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

  m_pCamera = m_pSceneMgr->createCamera("GameCamera");
  m_pCamera->setPosition(Vector3(200, 0, 500));
  m_pCamera->lookAt(Vector3(200, 0, 0));
  m_pCamera->setNearClipDistance(0.1);

  m_pCamera->setAspectRatio(Real(RenderEngine::Instance().m_pViewport->
    getActualWidth()) / Real(RenderEngine::Instance().m_pViewport->
      getActualHeight()));

  RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);

  // Generate Level
  levelman.sceneFromUrl(
//      "http://www.uni-koblenz.de",
//      "http://www.youtube.com/watch?v=urAyOKlgGDk",
//      "http://www.randomwebsite.com/cgi-bin/random.pl",
      "http://en.wikipedia.org/wiki/Special:Random",
//      "http://www.libpng.org/pub/png/png-rgba32.html",
//      "http://webkit.org/",
//      "the-space-station/index.html",
// 		"http://www.uni-koblenz.de/~lohoff/",
// 		"http://www.lubosz.de",
      m_pSceneMgr);

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
  this->level = level;
  System::Instance().logMessage("Generated level...");
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

  if (keyEventRef.key == OIS::KC_LEFT || keyEventRef.key == OIS::KC_RIGHT)
    Animation::Instance().deactivate();
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
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
//    m_pCamera->moveRelative(translateVector*5);
//  m_pCamera->moveRelative(translateVector);
  if (this->level != NULL && this->level->characterSceneNode != NULL) {
    Ogre::Vector3 pos = this->level->characterSceneNode->getPosition();
    pos.z += 300;
    pos.y += 50;
    m_pCamera->setPosition(pos);
  }
}

void LevelState::getInput() {
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
//    translateVector.x = -moveSpeed;
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
//    translateVector.x = moveSpeed;
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W))
//    translateVector.z = -moveSpeed;
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
//    translateVector.z = moveSpeed;

//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
//    m_pCamera->move(Ogre::Vector3(moveSpeed * 20.0, 0.0, 0.0));
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LEFT))
//    m_pCamera->move(Ogre::Vector3(-moveSpeed * 20.0, 0.0, 0.0));
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_UP))
//    m_pCamera->move(Ogre::Vector3(0.0, moveSpeed * 5.0, 0.0));
//
//  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_DOWN))
//    m_pCamera->move(Ogre::Vector3(0.0, -moveSpeed * 5.0, 0.0));
}

void LevelState::update(double timeSinceLastFrame) {
  m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;

  if (this->level != NULL)
    this->level->update(timeSinceLastFrame * 0.000001);
  else
    qDebug() << "ERROR! No Level";

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
