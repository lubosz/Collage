/*
 * InputManager.h
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#pragma once

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class Input {
public:

	OIS::InputManager* m_pInputMgr;
	OIS::Keyboard* m_pKeyboard;
	OIS::Mouse* m_pMouse;

	void initOis(OIS::KeyListener *pKeyListener = 0,
			OIS::MouseListener *pMouseListener = 0);

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	Input();
	virtual ~Input();
};

