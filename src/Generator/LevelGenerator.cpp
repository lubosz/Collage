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
  QStringList blacklist;
  blacklist << ".swf" << ".svg" << ".pdf" << ".png" << ".jpg" << ".jpeg"
            << ".zip" << ".rar" << ".torrent" << ".mp3" << ".avi" << ".mpg"
            << ".mpeg" << ".exe" << ".gz" << ".pls" << ".asx" << ".mov"
            << ".txt" << ".wav";
  foreach(QWebElement element, elements) {
//      qDebug() << "Adding door";
      QString url = element.attribute("href");
      i+=3;
      if (url.contains("#"))
        continue;
      bool cont = false;
      foreach(QString entry, blacklist) {
        if (url.contains(entry, Qt::CaseInsensitive))
          cont = true;
      }
      if (cont)
        continue;
      qDebug() << "Door for" << url;
      Actor *actor = new Actor();
      Ogre::Entity* door = sceneManager->createEntity("door.mesh");
      actor->sceneNode->attachObject(door);
    }
//  this->simulation->attachInteractionHandler(IT_DOOR, IT_GRAVITY,
//      new IHCharacterGravity(Ogre::Vector2(0., -9.81)));
//  this->simulation->attachInteractionHandler(IT_DOOR, IT_TERRAIN,
//      new IHCharacterTerrain());
}
