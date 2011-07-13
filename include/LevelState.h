/*
 * LevelState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#define BASE_URL "http://en.wikipedia.org/wiki/University_of_Koblenz_and_Landau"

#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreMaterialManager.h>

#include "DotSceneLoader.h"

#include "AppState.h"
#include "Simulation.h"
#include "LevelManager.h"

class LevelState : public AppState {
    Q_OBJECT

 public:
  Level *level;
  LevelState();

  DECLARE_APPSTATE_CLASS(LevelState)

  void enter();
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

  void update(double timeSinceLastFrame);

  QString url;

  public slots:
  void levelGenerated(Level *level);

 private:

  bool quit;
  float rotatedome;

  Ogre::Vector3 translateVector;
  Ogre::Real moveSpeed;
  Ogre::Degree rotateSpeed;
  float moveScale;
  Ogre::Degree rotScale;

  bool lMouseDown, rMouseDown;
};
