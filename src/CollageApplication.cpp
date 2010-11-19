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

CollageApplication::CollageApplication() {
}

CollageApplication::~CollageApplication() {
}

void CollageApplication::start() {
	//Wallpaper myWallPaperGenerator = Wallpaper();
	RenderEngine::Instance().initOgre("Collage");
	System::Instance().loadRecources();
	Input::Instance().initOis();
	UserInterface::Instance().initOgreBites();

    System::Instance().logMessage("Collage initialized");

	m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));

}

int main(int argc, char *argv[]) {
	//Qt part

	//QUrl url = QUrl("http://localhost/collage/");
	//QUrl url = QUrl("http://en.wikipedia.org/wiki/Special:Random");
	QUrl url = QUrl("http://wallbase.net/random");
	//QUrl url = QUrl("http://wallbase2.net/high-resolution/1c648993030030fc43c6879b051bd5d9/wallpaper-247292.jpg");
    QString fileName = "../Media/Textures/wall.png";

    //"http://wallbase.net/wallpaper/"
    QApplication a(argc, argv);
    FrameCapture capture;
    QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    capture.load(url, fileName);
    //qDebug("image: %s", capture.getFirstAttribute("a","href","http://wallbase.net/wallpaper/"));
    a.exec();

	CollageApplication collage;
	try {
		collage.start();
	}
	catch(const std::exception& e) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
}
