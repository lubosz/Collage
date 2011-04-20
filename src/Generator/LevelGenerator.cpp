/*
 *  Copyright 2010 The Collage Project
 */
#include "LevelGenerator.h"
#include "DefaultSimulation.h"
#include <QDebug>

LevelGenerator::LevelGenerator(QObject *parent)
  :
    QObject(parent) {
      this->simulation =
          static_cast<Simulation*>(new DefaultSimulation(this->sceneManager));
    }

QString LevelGenerator::getName() {
  return this->name;
}

void LevelGenerator::update(double timeSinceLastFrame) {
  this->simulation->update(timeSinceLastFrame);
}

void LevelGenerator::addDoors() {
  QWebElement document = webpage->mainFrame()->documentElement();
  QWebElementCollection elements = document.findAll("A");

  Ogre::SceneNode *root = this->sceneManager->getRootSceneNode();
  QStringList blacklist;
  blacklist << ".swf" << ".svg" << ".pdf" << ".png" << ".jpg" << ".jpeg"
            << ".zip" << ".rar" << ".torrent" << ".mp3" << ".avi" << ".mpg"
            << ".mpeg" << ".exe" << ".gz" << ".pls" << ".asx" << ".mov"
            << ".txt" << ".wav";
  foreach(QWebElement element, elements) {
      QString url = element.attribute("href");
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
      Ogre::Entity* door = sceneManager->createEntity("door.mesh");
    }
}
