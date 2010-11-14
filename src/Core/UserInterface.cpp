/*
 * UserInterface.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "UserInterface.h"

template<> UserInterface* Ogre::Singleton<UserInterface>::ms_Singleton = 0;

UserInterface::UserInterface() {
}

UserInterface::~UserInterface() {
	if (m_pTrayMgr)
		delete m_pTrayMgr;
}

void UserInterface::initOgreBites(
		Ogre::RenderWindow* window,
		OIS::Mouse* mouse
	) {
	m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", window, mouse, 0);
}
