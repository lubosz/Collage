/*
 * UserInterface.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "common.h"
#include "Singleton.h"
#include <OGRE/SdkTrays.h>

class UserInterface : public Singleton<UserInterface> {
 public:

	OgreBites::SdkTrayManager* m_pTrayMgr;

	UserInterface();
	virtual ~UserInterface();

	void initOgreBites(Ogre::RenderWindow* window, OIS::Mouse* mouse);
};

