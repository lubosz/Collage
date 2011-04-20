/*
 *  Copyright 2010 The Collage Project
 */
#pragma once

#include "LevelGenerator.h"

class DivBoxGenerator : public LevelGenerator {
 public:
    DivBoxGenerator();
    void makeOgreImage(
        QWebElement * element, const Ogre::String & textureName,
        unsigned faces = 1);
    Ogre::Vector3 attachNode(
        QWebElement * element,
        Ogre::SceneNode * parentNode,
        Ogre::Real scale,
        const Ogre::String & textureName,
        Ogre::Entity* cube,
        Ogre::Vector3 position);
    bool fits(QWebElement * element, int min, int max);
    float getScore(QWebPage *webpage);
    Level* generate(Ogre::SceneManager *sceneManager);
    void makeElementBoxes(
        const QWebElement& document,
        Ogre::Real scale,
        Ogre::Real step,
        std::vector<QString> tags,
        Ogre::String meshName,
        Ogre::SceneManager * sceneManager);
    void setPageRendering(const QSize & siteResolution);
    Ogre::MaterialPtr makeMaterial(Ogre::String name, Ogre::String textureName);
};
