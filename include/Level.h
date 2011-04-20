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
#include "Simulation.h"

class Level : public QObject {
  Q_OBJECT
  public:
    explicit Level(QObject *parent = 0);

    virtual float getScore(QWebPage *webpage) = 0;
    virtual void generate(Ogre::SceneManager *sceneManager) = 0;
    QString getName();
    virtual void update(double timeSinceLastFrame);

  protected:
    QString name;
    QWebPage *webpage;
    Ogre::SceneManager *sceneManager;
    Simulation *simulation;

    virtual void addDoors();

  signals:

  public slots:
};

#endif
