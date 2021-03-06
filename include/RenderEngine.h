/*
 * RenderEngine.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once
#include "Singleton.h"
#include <OGRE/Ogre.h>


class RenderEngine : public Singleton<RenderEngine> {
 public:

	bool initOgre(Ogre::String wndTitle);
	void updateOgre(double timeSinceLastFrame);


	Ogre::Viewport* m_pViewport;
	Ogre::Root* m_pRoot;
	Ogre::RenderWindow* m_pRenderWnd;

	RenderEngine();
	virtual ~RenderEngine();
};

