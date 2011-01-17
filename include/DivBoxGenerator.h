/*
 *  Copyright 2010 The Collage Project
 */
#pragma once

#include "LevelGenerator.h"

class DivBoxGenerator : public LevelGenerator {
 public:
    DivBoxGenerator();
    void makeOgreImage(
        QWebElement * element, const Ogre::String & textureName);
    Ogre::Real attachNode(
        QWebElement * element,
        Ogre::SceneNode * parentNode,
        Ogre::Real scale,
        Ogre::Real count,
        const Ogre::String & textureName,
        Ogre::Entity* cube);
    bool fits(QWebElement * element, unsigned min, unsigned max);
    float getScore(QWebPage *webpage);
    Level* generate(Ogre::SceneManager * sceneManager);
    void makeElementBoxes(
        const QWebElement& document,
        Ogre::Real scale,
        Ogre::Real step,
        QString tagName,
        Ogre::String meshName,
        Ogre::SceneManager * sceneManager);
    void setPageRendering(const QSize & siteResolution);
};
