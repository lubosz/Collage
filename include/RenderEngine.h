/*
 * RenderEngine.h
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"


class RenderEngine : public Ogre::Singleton<RenderEngine> {
public:

	bool initOgre(Ogre::String wndTitle);
	void updateOgre(double timeSinceLastFrame);

	Ogre::Root* m_pRoot;
	Ogre::RenderWindow* m_pRenderWnd;
	Ogre::Viewport* m_pViewport;
	Ogre::Log* m_pLog;
	Ogre::Timer* m_pTimer;

	RenderEngine();
	virtual ~RenderEngine();

private:
	RenderEngine(const RenderEngine&);
	RenderEngine& operator=(const RenderEngine&);

};

