/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include "GeneralLevel.h"

#include "RenderEngine.h"

GeneralLevel::GeneralLevel(QObject *parent)
  :
    Level(parent) {
      this->name = "GeneralLevel";
    }

float GeneralLevel::getScore(QWebPage *webpage) {
  this->webpage = webpage;
  return 100.0;
}

void GeneralLevel::generate(Ogre::SceneManager *sceneManager) {
  this->sceneManager = sceneManager;
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

  this->addDoors();
}
