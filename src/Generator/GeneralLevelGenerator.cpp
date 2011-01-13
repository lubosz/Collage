/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include "GeneralLevelGenerator.h"

#include "RenderEngine.h"

GeneralLevelGenerator::GeneralLevelGenerator(QObject *parent)
:
    LevelGenerator(parent) {
}

float GeneralLevelGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 100.0;
}

Level* GeneralLevelGenerator::generate(Ogre::SceneManager * sceneManager) {
    // uses this.webframe to generate Level, returns Level
//    Ogre::SceneManager* manager =
//            RenderEngine::Instance().m_pRoot->createSceneManager(
//            Ogre::ST_GENERIC, "GameSceneMgr");
//    Simulation *simulation = new Simulation(manager->getRootSceneNode());
//
//    manager->createLight("Light")->setPosition(75, 75, 75);
//
//	simulation->createActor("myActor1", AB_STATIC, Ogre::Vector2(0.0, 0.0));
//	simulation->createActor("myActor2", AB_STATIC, Ogre::Vector2(1.5, 0.0));
//	simulation->createActor("myActor3", AB_STATIC, Ogre::Vector2(0.0, 1.5));
//	simulation->createActor("myActor4", AB_DYNAMIC, Ogre::Vector2(1.5, 1.5));
  sceneManager->createLight("Light")->setPosition(75, 75, 75);

  Ogre::SceneNode* m_pOgreHeadNode;
  Ogre::Entity* m_pOgreHeadEntity;
  Ogre::MaterialPtr m_pOgreHeadMat;
  Ogre::MaterialPtr m_pOgreHeadMatHigh;


  m_pOgreHeadEntity = sceneManager->createEntity("Cube", "ogrehead.mesh");
  m_pOgreHeadNode = sceneManager->getRootSceneNode()->createChildSceneNode(
      "CubeNode");
  m_pOgreHeadNode->attachObject(m_pOgreHeadEntity);
  m_pOgreHeadNode->setPosition(Ogre::Vector3(0, 0, -25));

  m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
  m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);

  Level *level = new Level();
    return level;
}
