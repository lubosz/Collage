/*
 * System.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "System.h"

System::System() {
	//Init log
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

	//Load recources
    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("config/resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //Set timer
    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();



}

System::~System() {
	// TODO Auto-generated destructor stub
}

void System::logMessage(Ogre::String message){
	m_pLog->logMessage(message);
}
