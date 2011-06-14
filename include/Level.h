/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QWebFrame>
#include <QWebPage>
#include <QString>
#include <QWebElement>
#include "Level.h"
#include "DefaultSimulation.h"

using std::string;

class Level : public QObject {
  Q_OBJECT
  public:
    explicit Level(QObject *parent = 0);
	  explicit Level(Ogre::SceneManager *sceneManager);

    virtual float getScore(QWebPage *webpage) = 0;
    virtual void generate() = 0;
    QString getName();
    virtual void update(double timeSinceLastFrame);

    Ogre::SceneNode* characterSceneNode;
	std::vector<Door*> doors;

  protected:
    QString name;
    QWebPage *webpage;
    Ogre::SceneManager *sceneManager;
    DefaultSimulation *simulation;

    virtual void generateDoors();
    virtual void addCharacter();

  signals:

  public slots:
};

#endif
