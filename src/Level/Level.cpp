/*
 *  Copyright 2010 The Collage Project
 */
#include "Level.h"
#include "DefaultSimulation.h"
#include <QDebug>

Level::Level(QObject *parent)
  :
    QObject(parent) {
    }

Level::Level(Ogre::SceneManager *sceneManager) {
  this->simulation =
    static_cast<Simulation*>(new DefaultSimulation(sceneManager));
}

QString Level::getName() {
  return this->name;
}

void Level::update(double timeSinceLastFrame) {
  this->simulation->update(timeSinceLastFrame);
}

void Level::addDoors() {
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
