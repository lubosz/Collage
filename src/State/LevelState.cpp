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
  rotatedome = 0;
  moveSpeed = 0.5f;
  rotateSpeed = -0.5f;

  lMouseDown = false;
  rMouseDown = false;
  quit = false;

  this->level = NULL;
  this->url = "wikidump4/index.html";
}

void LevelState::enter() {
  Input::Instance().m_pMouse->setBuffered(false);

  // Set up Camera
  System::Instance().logMessage("Entering LevelState...");

  if (!RenderEngine::Instance().m_pRoot->hasSceneManager("LevelSceneManager")) {
    m_pSceneMgr = RenderEngine::Instance().m_pRoot->createSceneManager(
            Ogre::ST_GENERIC, "LevelSceneManager");
  } else {
    m_pSceneMgr =
        RenderEngine::Instance().m_pRoot->getSceneManager("LevelSceneManager");
    m_pSceneMgr->clearScene();
  }

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

  m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);
  m_pSceneMgr->createLight("Light1")->setPosition(-75, 100, -75);
  m_pSceneMgr->createLight("Light2")->setPosition(-75, 120, 75);
  m_pSceneMgr->createLight("Light3")->setPosition(75, 130, -75);

  // Generate Level
  levelman.sceneFromUrl(
//      "http://www.uni-koblenz.de",
//      "http://www.youtube.com/watch?v=urAyOKlgGDk",
//      "http://www.randomwebsite.com/cgi-bin/random.pl",
//      "http://en.wikipedia.org/wiki/Special:Random",
//      "http://www.libpng.org/pub/png/png-rgba32.html",
//      "http://webkit.org/",
//      "the-space-station/index.html",
// 		"http://www.uni-koblenz.de/~lohoff/",
// 		"http://www.lubosz.de",
		this->url,
        m_pSceneMgr);

  // buildGUI();
}

bool LevelState::pause() {
  System::Instance().logMessage("Pausing LevelState...");

  return true;
}

void LevelState::resume() {
  System::Instance().logMessage(
      "Resuming LevelState...");

  // buildGUI();

  RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);
  quit = false;
}

void LevelState::exit() {
  Input::Instance().m_pMouse->setBuffered(true);
  System::Instance().logMessage("Leaving LevelState...");

  m_pSceneMgr->destroyCamera(m_pCamera);
//  if (m_pSceneMgr) {
//    RenderEngine::Instance().m_pRoot->destroySceneManager(
//        m_pSceneMgr);
//  }
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
  Input::Instance().keyPressed();
  return true;
}

bool LevelState::keyReleased(const OIS::KeyEvent &keyEventRef) {
  if (keyEventRef.key == OIS::KC_LEFT || keyEventRef.key == OIS::KC_RIGHT)
    Animation::Instance().deactivate();
  Input::Instance().keyReleased();
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

  // you can lose the game
  if (m_pCamera->getPosition().y < -1000)
    pushAppState(findByName("LostState"));

  moveScale = moveSpeed * timeSinceLastFrame;
  rotScale = rotateSpeed * timeSinceLastFrame;

  translateVector = Vector3::ZERO;

  rotatedome += timeSinceLastFrame* 0.0000001;
  Ogre::Quaternion rotation = Ogre::Quaternion(
      Ogre::Radian(rotatedome), Ogre::Vector3::UNIT_Y);

  m_pSceneMgr->getSkyDomeNode()->setOrientation(rotation);

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
