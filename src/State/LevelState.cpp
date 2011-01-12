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
  moveSpeed = 0.1f;
  rotateSpeed = 0.3f;

  lMouseDown = false;
  rMouseDown = false;
  quit = false;

  connect(&genman, SIGNAL(levelGenerated(Level*)),
          this, SLOT(levelGenerated(Level*)));
}

void LevelState::enter() {
  genman.requestWebpage("http://www.example.net");
  System::Instance().logMessage(
      "Entering LevelState...");

  m_pSceneMgr
      = RenderEngine::Instance().m_pRoot->createSceneManager(
          Ogre::ST_GENERIC, "LevelSceneManager");
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
  System::Instance().logMessage("Leaving LevelState...");

  m_pSceneMgr->destroyCamera(m_pCamera);
  if (m_pSceneMgr)
    RenderEngine::Instance().m_pRoot->destroySceneManager(
        m_pSceneMgr);
}

void LevelState::createScene() {
  m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);


  Ogre::SceneNode* m_pOgreHeadNode;
  Ogre::Entity* m_pOgreHeadEntity;
  Ogre::MaterialPtr m_pOgreHeadMat;
  Ogre::MaterialPtr m_pOgreHeadMatHigh;


  m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
  m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
      "CubeNode");
  m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
  m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

  m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
  m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
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

  if (rMouseDown) {
    m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
    m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
  }

  return true;
}

bool LevelState::mousePressed(
    const OIS::MouseEvent &evt,
    OIS::MouseButtonID id
  ) {
  if (UserInterface::Instance().m_pTrayMgr->injectMouseDown(evt, id))
    return true;

  if (id == OIS::MB_Left) {
    onLeftPressed(evt);
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

void LevelState::onLeftPressed(const OIS::MouseEvent &evt) {

}

void LevelState::moveCamera() {
  if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
    m_pCamera->moveRelative(translateVector*5);
  m_pCamera->moveRelative(translateVector);
}

void LevelState::getInput() {
    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_A))
      translateVector.x = -moveScale;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_D))
      translateVector.x = moveScale;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_W))
      translateVector.z = -moveScale;

    if (Input::Instance().m_pKeyboard->isKeyDown(OIS::KC_S))
      translateVector.z = moveScale;
}

void LevelState::update(double timeSinceLastFrame) {
  m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
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
}

void LevelState::buildGUI() {
  UserInterface::Instance().m_pTrayMgr->showFrameStats(
      OgreBites::TL_BOTTOMLEFT);
  UserInterface::Instance().m_pTrayMgr->createLabel(
      OgreBites::TL_TOP, "GameLbl", "Level mode", 250);
  UserInterface::Instance().m_pTrayMgr->showCursor();

}
