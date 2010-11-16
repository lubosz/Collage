/*
 * System.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "System.h"
#include <stdio.h>
#include <string>

using std::string;

System::System() {
    logMessage("Starting System");
}

System::~System() {
}
void System::initTimer() {
    // Set timer
    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();
}

void System::init() {
	initResources();
	initTimer();
    Ogre::LogManager::getSingleton().createLog("Collage.log", true, true, false);
}

void System::initResources() {
	// Load recources
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().
            		addResourceLocation(archName, typeName, secName);
        }
    }
}

void System::logMessage(string message) {
	//TODO: Segfault
	//m_pLog->logMessage(message);
	Ogre::LogManager::getSingleton().logMessage(message);
	printf("[\x1b[1;31mCollage\e[m]: %s\n", message.c_str()); // red
}
