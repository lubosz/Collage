/*
 *  Copyright 2010 The Collage Project
 */
#include "LevelGenerator.h"
#include <QDebug>

LevelGenerator::LevelGenerator(QObject *parent)
  :
    QObject(parent) {
    }

QString LevelGenerator::getName() {
  return this->name;
}

void LevelGenerator::addDoors() {
  QWebElement document = webpage->mainFrame()->documentElement();
  QWebElementCollection elements = document.findAll("A");
  int count = elements.count();

  Ogre::SceneNode *root = this->sceneManager->getRootSceneNode();
  int i = 0;
  foreach(QWebElement element, elements) {
//      qDebug() << "Adding door";
      Ogre::Entity* door = sceneManager->createEntity("door.mesh");
      Ogre::SceneNode * node = root->createChildSceneNode();
      node->attachObject(door);
      node->setPosition(i, 0, 0);
      ++i;
    }
}
