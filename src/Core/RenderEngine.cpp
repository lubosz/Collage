/*
 * RenderEngine.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "RenderEngine.h"
#include "System.h"

RenderEngine::RenderEngine() {
    m_pRoot = 0;
	m_pRenderWnd = 0;
	m_pViewport = 0;
}

RenderEngine::~RenderEngine() {
    System::Instance().logMessage("Shutdown Render Engine...");

    // Causes segfault
	// if (m_pRoot)
	//	delete m_pRoot;
}

void RenderEngine::updateOgre(double timeSinceLastFrame) {
}

bool RenderEngine::initOgre(Ogre::String wndTitle) {


    m_pRoot = new Ogre::Root();
    System::Instance().initResources();
    //System::Instance().initLogging();

    if (!m_pRoot->showConfigDialog())
        return false;

    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    System::Instance().logMessage("Starting Render Engine");

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    m_pViewport->setCamera(0);

    m_pRenderWnd->setActive(true);

    System::Instance().initTimer();


    return true;
}
