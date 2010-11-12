/*
 * RenderEngine.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "RenderEngine.h"
#include "OgreColourValue.h"

template<> RenderEngine* Ogre::Singleton<RenderEngine>::ms_Singleton = 0;


RenderEngine::RenderEngine() {
    m_pRoot = 0;
	m_pRenderWnd = 0;
	m_pViewport = 0;
	m_pLog = 0;
	m_pTimer = 0;




}

RenderEngine::~RenderEngine() {
	// TODO Auto-generated destructor stub
}

void RenderEngine::updateOgre(double timeSinceLastFrame)
{
}

bool RenderEngine::initOgre(Ogre::String wndTitle)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    if(!m_pRoot->showConfigDialog())
        return false;
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pViewport = m_pRenderWnd->addViewport(0);
    //m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    m_pViewport->setCamera(0);





    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

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



    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    m_pRenderWnd->setActive(true);

    return true;
}
