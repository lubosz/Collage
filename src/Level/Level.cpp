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
}

void Level::generateDoors() {
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
      if (url.contains("#") || url.contains("http"))
        continue;
      foreach(QString entry, blacklist) {
        if (url.contains(entry, Qt::CaseInsensitive))
		  continue;
      }
      qDebug() << "Door for" << url;
	  qDebug() << "DOOR GEOMETRY:" << element.geometry();
	  if ((element.geometry().width() > 0) && (element.geometry().height() > 0)) {
        DefaultSimulation *simulation =
          static_cast<DefaultSimulation*>(this->simulation);
        Door *doorActor = simulation->doorFactory->createActor();
	    doorActor->geometry = element.geometry();
		doors.push_back(doorActor);
	  }
	  else
		qDebug() << "Door REJECTED!";

    }
	qDebug() << "\n";
}

