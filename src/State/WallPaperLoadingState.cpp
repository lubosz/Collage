/*
 * MenuState.cpp
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "WallPaperLoadingState.h"
#include "FrameCapture.h"
#include <QtCore>

// using namespace Ogre;

WallPaperLoadingState::WallPaperLoadingState() {
	m_bQuit = false;
	m_FrameEvent = Ogre::FrameEvent();
}

void WallPaperLoadingState::enter() {
	System::Instance().logMessage("Entering Wallpaper Leech State...");

	createScene();
	popAllAndPushAppState(findByName("MenuState"));
}

void WallPaperLoadingState::createScene() {
  // nsfw
//  QUrl url = QUrl("http://wallbase.net/random/all/eqeq/1920x1200/1.60/001/20");
  QUrl url = QUrl("http://wallbase.net/random");
  FrameCapture capture;
//  capture.saveWebRender(QUrl("http://en.wikipedia.org/wiki/Special:Random"),
//      "../Media/Textures/wall.png");
//  capture.saveWallPaper(url, "../Media/Textures/wall.jpg");

  System::Instance().logMessage("Wallpaper downloaded");
}

void WallPaperLoadingState::exit() {
	System::Instance().logMessage("Leaving Wallpaper...");
}

bool WallPaperLoadingState::keyPressed(const OIS::KeyEvent &keyEventRef) {
	return true;
}

bool WallPaperLoadingState::keyReleased(const OIS::KeyEvent &keyEventRef) {
	return true;
}

bool WallPaperLoadingState::mouseMoved(const OIS::MouseEvent &evt) {
	return true;
}

bool WallPaperLoadingState::mousePressed(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	return true;
}

bool WallPaperLoadingState::mouseReleased(
		const OIS::MouseEvent &evt,
		OIS::MouseButtonID id
	) {
	return true;
}

void WallPaperLoadingState::update(double timeSinceLastFrame) {
}

void WallPaperLoadingState::buttonHit(OgreBites::Button* button) {
}
