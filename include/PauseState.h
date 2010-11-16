/*
 * PauseState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppState.h"

class PauseState : public AppState {
 public:
    PauseState();

    DECLARE_APPSTATE_CLASS(PauseState)

    void enter();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button* button);
    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

    void update(double timeSinceLastFrame);

 private:
    bool m_bQuit;
};
