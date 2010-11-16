/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "System.h"

using namespace Ogre;


void basicOgre() {

	Ogre::Viewport* m_pViewport;
	Ogre::Root* m_pRoot;
	Ogre::RenderWindow* m_pRenderWnd;

    m_pRoot = new Ogre::Root();
    System::Instance().init();
    m_pRoot->showConfigDialog();

    m_pRenderWnd = m_pRoot->initialise(true, "Basic Ogre");
    m_pRenderWnd->setActive(true);
    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
    m_pViewport->setCamera(0);
	System::Instance().loadRecources();

	Ogre::SceneManager* m_pSceneMgr;
	Ogre::Camera* m_pCamera;
	m_pSceneMgr = m_pRoot->createSceneManager(
			Ogre::ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(5, 60, 60));
	m_pCamera->lookAt(Vector3(5, 20, 0));
	m_pCamera->setNearClipDistance(5);

	m_pCamera->setAspectRatio(Real(
			m_pViewport->getActualWidth()) / Real(
			m_pViewport->getActualHeight()));

	m_pViewport->setCamera(m_pCamera);

	Ogre::SceneNode* m_pOgreHeadNode;
	Ogre::Entity* m_pOgreHeadEntity;

	m_pOgreHeadEntity = m_pSceneMgr->createEntity("Cube", "ogrehead.mesh");
	m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
	m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
	m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

	int timeSinceLastFrame = 1;
	int startTime = 0;

	while (true) {
		startTime = System::Instance().m_pTimer->getMillisecondsCPU();
		m_pOgreHeadNode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(float(startTime%10000)/100000.0));
		m_pRoot->renderOneFrame();

		timeSinceLastFrame = System::Instance().m_pTimer->getMillisecondsCPU() - startTime;

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
