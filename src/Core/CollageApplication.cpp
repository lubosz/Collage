/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "CollageApplication.h"
#include "WallPaperLoadingState.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"
#include "System.h"
#include "MenuState.h"
#include "PauseState.h"
#include "LostState.h"
#include "LoadingState.h"
#include "SimulationTestState.h"
#include "LevelState.h"
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>

CollageApplication::CollageApplication(int& argc, char** argv)  // NOLINT
: QApplication(argc, argv) {
	QCoreApplication::setApplicationName("Collage");
}

CollageApplication::~CollageApplication() {
}


int CollageApplication::exec() {
#ifdef AUDIO
	// audio
    Phonon::AudioOutput *audioOutput =
        new Phonon::AudioOutput(Phonon::MusicCategory);
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject();

    Phonon::MediaSource source("http://89.179.179.5:8040");
    mediaObject->setCurrentSource(source);
    mediaObject->play();

    Phonon::createPath(mediaObject, audioOutput);
#endif

	RenderEngine::Instance().initOgre("Collage");
  System::Instance().loadRecources();
  Input::Instance().initOis();
  UserInterface::Instance().initOgreBites();

  System::Instance().logMessage("Collage initialized");

  AppStateManager::Instance();

  MenuState::create("MenuState");
  LostState::create("LostState");
  PauseState::create("PauseState");
  LoadingState::create("LoadingState");
  WallPaperLoadingState::create("WallPaperLoadingState");
  SimulationTestState::create("SimulationTestState");
  LevelState::create("LevelState");

  System::Instance().logMessage("Appstates initialized");
  AppStateManager::Instance().start("LevelState");
}



