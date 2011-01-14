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
  Ogre::Real scale = .01;
  foreach(QWebElement element, elements) {
    if (element.geometry().width() != 0 && element.geometry().height() != 0) {
      qDebug() << "Some Div " << element.geometry();

      Ogre::Real width = element.geometry().width()*scale;
      Ogre::Real height = element.geometry().height()*scale;

      node = sceneManager->getRootSceneNode()->createChildSceneNode();
      cube = sceneManager->createEntity("Cube.mesh");
      node->attachObject(cube);
      node->setPosition(
          Ogre::Vector3(
              -element.geometry().left()*scale,
              -element.geometry().top()*scale,
              count));
      node->setScale(width, height, 1.0);
      count+=10;
    }
  }

  sceneManager->createLight("Light")->setPosition(75, 75, 75);

  Level *level = new Level();
  return level;
}
