/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include <QWebElement>
#include <QWebPage>
#include "DivBoxGenerator.h"

#include "RenderEngine.h"

DivBoxGenerator::DivBoxGenerator() {}

float DivBoxGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 10.0;
}

Level* DivBoxGenerator::generate(Ogre::SceneManager * sceneManager) {

  Ogre::SceneNode* node;
  Ogre::Entity* cube;



  QWebElement document = webpage->mainFrame()->documentElement();
  QWebElementCollection elements = document.findAll("div");
  Ogre::Real count = 0;
  foreach(QWebElement element, elements){

    qDebug() << "Some Div " << element.geometry();

    Ogre::Real width = element.geometry().width() / 100.0;
    Ogre::Real height = element.geometry().height() / 100.0;

    node = sceneManager->getRootSceneNode()->createChildSceneNode();
    cube = sceneManager->createEntity("Cube.mesh");
    node->attachObject(cube);
    node->setPosition(Ogre::Vector3(element.geometry().left(), element.geometry().top(), count));
    node->setScale(width,height,1.0);
    count+=10;
  }
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


//  Ogre::MaterialPtr m_pOgreHeadMat;
//  m_pOgreHeadMat = m_pOgreHeadEntity->getSubEntity(1)->getMaterial();
//  m_pOgreHeadMatHigh = m_pOgreHeadMat->clone("OgreHeadMatHigh");
//  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
//  m_pOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);

  Level *level = new Level();
    return level;
}
