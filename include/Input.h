/*
 * InputManager.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include "Singleton.h"

class Input: public Singleton<Input> {
 public:

	OIS::InputManager* m_pInputMgr;
	OIS::Keyboard* m_pKeyboard;
	OIS::Mouse* m_pMouse;

	void initOis(OIS::KeyListener *pKeyListener = 0,
			OIS::MouseListener *pMouseListener = 0);

	bool keyPressed();
	bool keyReleased();

	bool mouseMoved();
	bool mousePressed();
	bool mouseReleased();

	Input();
	virtual ~Input();
};

