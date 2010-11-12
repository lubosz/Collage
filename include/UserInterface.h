/*
 * UserInterface.h
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

class UserInterface : public Ogre::Singleton<UserInterface> {
public:

	OgreBites::SdkTrayManager* m_pTrayMgr;

	UserInterface();
	virtual ~UserInterface();

	void initOgreBites(Ogre::RenderWindow* window, OIS::Mouse* mouse);
};

