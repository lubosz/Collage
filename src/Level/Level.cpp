/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include "Level.h"
#include "DefaultSimulation.h"
#include "DotSceneLoader.h"
#include "Animation.h"

Level::Level(QObject *parent)
  :
    QObject(parent) {
    }

Level::Level(Ogre::SceneManager *sceneManager) {
  this->sceneManager = sceneManager;
  this->simulation = new DefaultSimulation(sceneManager);
}

QString Level::getName() {
  return this->name;
}

void Level::update(double timeSinceLastFrame) {
  this->simulation->update(timeSinceLastFrame);
}


void Level::addCharacter() {
  sceneManager->createLight("Light")->setPosition(75, 75, 75);
  sceneManager->createLight("Light1")->setPosition(-75, 100, -75);
  sceneManager->createLight("Light2")->setPosition(-75, 120, 75);
  sceneManager->createLight("Light3")->setPosition(75, 130, -75);

  DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
  pDotSceneLoader->parseDotScene(
      "papercraft_man_line_running.scene",
      "General", sceneManager, sceneManager->getRootSceneNode());
    delete pDotSceneLoader;

  Animation::Instance().activate(sceneManager, "arm_left");
  Animation::Instance().activate(sceneManager, "arm_right");
  Animation::Instance().activate(sceneManager, "chest");
  Animation::Instance().activate(sceneManager, "leg_left");
  Animation::Instance().activate(sceneManager, "leg_right");
  Animation::Instance().activate(sceneManager, "pants");
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
      DefaultSimulation *simulation =
        static_cast<DefaultSimulation*>(this->simulation);
      Door *doorActor = simulation->doorFactory->createActor();
      this->placeDoor(doorActor, element.geometry());
    }
}

void Level::placeDoor(Door *doorActor, QRect geom) {
}
