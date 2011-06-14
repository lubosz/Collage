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

  m_pAppStateManager = new AppStateManager();

  MenuState::create(m_pAppStateManager, "MenuState");
  PauseState::create(m_pAppStateManager, "PauseState");
  LoadingState::create(m_pAppStateManager, "LoadingState");
  WallPaperLoadingState::create(m_pAppStateManager, "WallPaperLoadingState");
  SimulationTestState::create(m_pAppStateManager, "SimulationTestState");
  LevelState::create(m_pAppStateManager, "LevelState");

  System::Instance().logMessage("Appstates initialized");
  m_pAppStateManager->start(m_pAppStateManager->findByName("LoadingState"));
}



