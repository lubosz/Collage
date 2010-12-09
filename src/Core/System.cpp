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
    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();
}

void System::init() {
	initResources();
	initTimer();
    //Ogre::LogManager::getSingleton().createLog("Collage.log", true, true, false);
}

void System::initResources() {
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

void System::loadRecources() {
    System::Instance().logMessage("Loading recources");
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void System::logMessage(string message) {

	printf("[\e[%sCollage\e[m]: ", bashGreen.c_str()); // red
	Ogre::LogManager::getSingleton().logMessage(message);

}
