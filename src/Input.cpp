/*
 * InputManager.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "Input.h"
#include "UserInterface.h"
#include "RenderEngine.h"

template<> Input* Ogre::Singleton<Input>::ms_Singleton = 0;

Input::Input() {
	m_pInputMgr = 0;
	m_pKeyboard = 0;
	m_pMouse = 0;

}

Input::~Input() {

    if(m_pInputMgr)		OIS::InputManager::destroyInputSystem(m_pInputMgr);
}

void Input::initOis(OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener){
	unsigned long hWnd = 0;

    RenderEngine::getSingleton().m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    OIS::ParamList paramList;

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = RenderEngine::getSingleton().m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width  = RenderEngine::getSingleton().m_pRenderWnd->getWidth();

    m_pKeyboard->setEventCallback(pKeyListener);
    m_pMouse->setEventCallback(pMouseListener);

}

bool Input::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        RenderEngine::getSingleton().m_pRenderWnd->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
        return true;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if(UserInterface::getSingleton().m_pTrayMgr->isLogoVisible())
        {
        	UserInterface::getSingleton().m_pTrayMgr->hideFrameStats();
        	UserInterface::getSingleton().m_pTrayMgr->hideLogo();
        }
        else
        {
        	UserInterface::getSingleton().m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        	UserInterface::getSingleton().m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
        }
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool Input::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool Input::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool Input::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool Input::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}
