/*
 * LevelState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppState.h"
#include "DotSceneLoader.h"

#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreMaterialManager.h>

#include "LevelGeneratorManager.h"

class LevelState : public AppState {
    Q_OBJECT
 public:
  LevelState();

  DECLARE_APPSTATE_CLASS(LevelState)

  void enter();
  void createScene();
  void exit();
  bool pause();
  void resume();

  void moveCamera();
  void getInput();
  void buildGUI();

  bool keyPressed(const OIS::KeyEvent &keyEventRef);
  bool keyReleased(const OIS::KeyEvent &keyEventRef);

  bool mouseMoved(const OIS::MouseEvent &arg);
  bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  void onLeftPressed(const OIS::MouseEvent &evt);

  void update(double timeSinceLastFrame);
  LevelGeneratorManager genman;
  public slots:
  void levelGenerated(Level *level);

 private:

  bool quit;

  Ogre::Vector3 translateVector;
  Ogre::Real moveSpeed;
  Ogre::Degree rotateSpeed;
  float moveScale;
  Ogre::Degree rotScale;

  bool lMouseDown, rMouseDown;
};
