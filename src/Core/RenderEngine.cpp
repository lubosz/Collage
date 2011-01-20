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
}

void RenderEngine::updateOgre(double timeSinceLastFrame) {
}

bool RenderEngine::initOgre(Ogre::String wndTitle) {
    m_pRoot = new Ogre::Root();
    System::Instance().init();
#ifndef WITH_FIXBLACKSCREEN
// TODO(lubosz): Race condition? Black screen
    if (!m_pRoot->restoreConfig())
#endif
    	if (!m_pRoot->showConfigDialog())
    		exit(0);

    System::Instance().logMessage("Setting up window");
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    m_pRenderWnd->setActive(true);
    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
    m_pViewport->setCamera(0);

    return true;
}
