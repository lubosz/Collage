/*
 * UserInterface.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "UserInterface.h"
#include "System.h"
#include "RenderEngine.h"

UserInterface::UserInterface() {
}

UserInterface::~UserInterface() {
	if (m_pTrayMgr)
		delete m_pTrayMgr;
}

void UserInterface::initOgreBites() {
	System::Instance().logMessage("Starting Ogre Bites");
	m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", RenderEngine::Instance(). m_pRenderWnd, Input::Instance().m_pMouse, 0);
	m_pTrayMgr->getTrayContainer(OgreBites::TL_NONE)->hide();
}
