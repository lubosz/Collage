/*
 *  Copyright 2010 The Collage Project
 */
#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QObject>
#include <QWebFrame>
#include <QWebPage>
#include <QString>
#include <QWebElement>
#include "Level.h"
#include "Simulation.h"

class LevelGenerator : public QObject {
  Q_OBJECT
  public:
    explicit LevelGenerator(QObject *parent = 0);

    virtual float getScore(QWebPage *webpage) = 0;
    virtual Level* generate(Ogre::SceneManager *sceneManager,
                            Simulation *simulation) = 0;
    QString getName();

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
