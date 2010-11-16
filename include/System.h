/*
 * System.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once
#include "Singleton.h"
#include <OGRE/Ogre.h>
#include <string>

using std::string;

const string bashRed = "1;31m";
const string bashGreen = "1;32m";

class System : public Singleton<System> {
 public:
	System();
	virtual ~System();

	void logMessage(Ogre::String message);
	void init();
	void loadRecources();

	Ogre::Timer* m_pTimer;

private:

	Ogre::Log* m_pLog;

	void initTimer();
	void initLogging();
	void initResources();


};
