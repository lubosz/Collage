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
#include "Wallpaper.h"
#include "FrameCapture.h"
#include <QtGui>

CollageApplication::CollageApplication(int& argc, char** argv)	:QApplication(argc, argv){
	//QUrl url = QUrl("http://en.wikipedia.org/wiki/Special:Random");
    //qDebug("image: %s", capture.getFirstAttribute("a","href","http://wallbase.net/wallpaper/"));
//    QObject::connect(m_pAppStateManager, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    //QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
}

CollageApplication::~CollageApplication() {
}


int CollageApplication::exec()
{
	RenderEngine::Instance().initOgre("Collage");
	System::Instance().loadRecources();
	Input::Instance().initOis();
	UserInterface::Instance().initOgreBites();

    System::Instance().logMessage("Collage initialized");

//	QUrl url = QUrl("http://wallbase.net/random/all/eqeq/1920x1200/1.60/001/20"); //nsfw
	QUrl url = QUrl("http://wallbase.net/random");
    FrameCapture capture;
//    capture.saveWebRender(url , "../Media/Textures/wall.jpg");
    capture.saveWallPaper(url , "../Media/Textures/wall.jpg");

    System::Instance().logMessage("Wallpaper downloaded");

    m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");
    System::Instance().logMessage("Appstates initialized");
	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}



