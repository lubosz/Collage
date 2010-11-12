/*
 * RenderEngine.h
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Singleton.h"


class RenderEngine : public Singleton<RenderEngine> {
public:

	bool initOgre(Ogre::String wndTitle);
	void updateOgre(double timeSinceLastFrame);

	Ogre::Root* m_pRoot;
	Ogre::RenderWindow* m_pRenderWnd;
	Ogre::Viewport* m_pViewport;


	RenderEngine();
	virtual ~RenderEngine();

};

