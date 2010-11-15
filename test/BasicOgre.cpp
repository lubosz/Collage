/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "CollageApplication.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"
#include "System.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"


void basicOgre() {

	RenderEngine::Instance().initOgre("Collage");

	Ogre::SceneManager* m_pSceneMgr;
	Ogre::Camera* m_pCamera;
	m_pSceneMgr = RenderEngine::Instance().m_pRoot->createSceneManager(
			Ogre::ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(5, 60, 60));
	m_pCamera->lookAt(Vector3(5, 20, 0));
	m_pCamera->setNearClipDistance(5);

	m_pCamera->setAspectRatio(Real(
			RenderEngine::Instance().m_pViewport->getActualWidth()) / Real(
			RenderEngine::Instance().m_pViewport->getActualHeight()));

	RenderEngine::Instance().m_pViewport->setCamera(m_pCamera);

	Ogre::SceneNode* m_pOgreHeadNode;
	Ogre::Entity* m_pOgreHeadEntity;
	Ogre::MaterialPtr m_pOgreHeadMat;
	Ogre::MaterialPtr m_pOgreHeadMatHigh;

	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
	m_pOgreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);
	m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(
			"CubeNode");
	m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
	m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

	m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
	m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
	m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);

	int timeSinceLastFrame = 1;
	int startTime = 0;

	int rotation =0;

	while (true) {
		startTime = System::Instance().m_pTimer->getMillisecondsCPU();

		m_pOgreHeadNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(rotation));

		RenderEngine::Instance().updateOgre(timeSinceLastFrame);
		RenderEngine::Instance().m_pRoot->renderOneFrame();

		timeSinceLastFrame = System::Instance().m_pTimer->getMillisecondsCPU()
				- startTime;

		rotation++;
	}


}

int main(int argc, char *argv[]) {
	try {
		basicOgre();
	}
	catch(const std::exception& e) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
}
