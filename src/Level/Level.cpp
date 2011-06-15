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
  assert(sceneManager != NULL);
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
}

void Level::generateDoors() {
  QWebElement document = webpage->mainFrame()->documentElement();
  QWebElementCollection elements = document.findAll("A");

//  Ogre::SceneNode *root = this->sceneManager->getRootSceneNode();
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
	  if ((element.geometry().width() > 0) && (element.geometry().height() > 0)) {
        DefaultSimulation *simulation =
          static_cast<DefaultSimulation*>(this->simulation);
        Door *doorActor = simulation->doorFactory->createActor();
	    doorActor->geometry = element.geometry();
		doorActor->url = url;
		doors.push_back(doorActor);
	  }
    }
	qDebug() << "\n";
}

