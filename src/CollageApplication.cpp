/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "CollageApplication.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"
#include "System.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"

CollageApplication::CollageApplication() {
}

CollageApplication::~CollageApplication() {
}

void CollageApplication::start() {

    System::Instance().init();
	RenderEngine::Instance().initOgre("Collage");
	System::Instance().loadRecources();
	Input::Instance().initOis();
	UserInterface::Instance().initOgreBites(
		RenderEngine::Instance(). m_pRenderWnd,
		Input::Instance().m_pMouse
	);

    System::Instance().logMessage("Collage initialized");

	m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));

}

int main(int argc, char *argv[]) {
	CollageApplication collage;
	try {
		collage.start();
	}
	catch(const std::exception& e) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
}
