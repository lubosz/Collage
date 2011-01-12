/*
 * GameState.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppState.h"

#include "DotSceneLoader.h"



#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include <LevelGeneratorManager.h>

enum QueryFlags {
    OGRE_HEAD_MASK	= 1<<0,
    CUBE_MASK		= 1<<1
};

class GameState : public AppState {
    Q_OBJECT
 public:
	GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera();
	void getInput();
	void buildGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void onLeftPressed(const OIS::MouseEvent &evt);
    void itemSelected(OgreBites::SelectMenu* menu);

	void update(double timeSinceLastFrame);
	LevelGeneratorManager genman;
public slots:
	void levelGenerated(Level *level);

 private:
	Ogre::SceneNode* m_pOgreHeadNode;
	Ogre::Entity* m_pOgreHeadEntity;
	Ogre::MaterialPtr m_pOgreHeadMat;
	Ogre::MaterialPtr m_pOgreHeadMatHigh;

    OgreBites::ParamsPanel* m_pDetailsPanel;
	bool m_bQuit;

	Ogre::Vector3 m_TranslateVector;
	Ogre::Real m_MoveSpeed;
	Ogre::Degree m_RotateSpeed;
	float m_MoveScale;
	Ogre::Degree m_RotScale;

	Ogre::RaySceneQuery* m_pRSQ;
	Ogre::SceneNode* m_pCurrentObject;
	Ogre::Entity* m_pCurrentEntity;
	bool m_bLMouseDown, m_bRMouseDown;
	bool m_bSettingsMode;
};
