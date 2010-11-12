/*
 * UserInterface.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "UserInterface.h"

template<> UserInterface* Ogre::Singleton<UserInterface>::ms_Singleton = 0;

UserInterface::UserInterface() {
	// TODO Auto-generated constructor stub

}

UserInterface::~UserInterface() {
	// TODO Auto-generated destructor stub
}

void UserInterface::initOgreBites(Ogre::RenderWindow* window, OIS::Mouse* mouse){
    m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", window, mouse, 0);
}
