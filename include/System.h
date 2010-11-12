/*
 * System.h
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#pragma once
#include "common.h"
#include "Singleton.h"


class System : public Singleton<System>{
public:
	System();
	virtual ~System();

	Ogre::Log* m_pLog;
	Ogre::Timer* m_pTimer;

	void logMessage(Ogre::String message);
};
