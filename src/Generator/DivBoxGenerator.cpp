/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include <QWebElement>
#include <QWebPage>
#include <QPainter>
#include "DivBoxGenerator.h"

#include "RenderEngine.h"

DivBoxGenerator::DivBoxGenerator() {
  this->name = "Deiner Mudder ihr Generator";
}

float DivBoxGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 4.0;
}

void DivBoxGenerator::makeOgreImage(QWebElement * element,
    const Ogre::String & textureName) {
  QImage image(element->geometry().size(), QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  element->render(&painter);
  painter.end();

  Ogre::TextureManager::getSingleton().remove(textureName);
  Ogre::TextureManager::getSingleton().loadImage(textureName, "General",
      Ogre::Image().loadDynamicImage(image.bits(), image.width(),
          image.height(), 1, Ogre::PF_A8R8G8B8));
}

void DivBoxGenerator::attachNode(
    QWebElement * element,
    Ogre::SceneNode * parentNode,
    Ogre::Real scale,
    Ogre::Real count,
    const Ogre::String & textureName,
    Ogre::Entity* cube) {

  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
      "PageMat" + Ogre::StringConverter::toString(count),
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  material.get()->getTechnique(0)-> getPass(0)->createTextureUnitState(
      textureName);

  Ogre::Real width = element->geometry().width()*scale;
  Ogre::Real height = element->geometry().height()*scale;

  Ogre::SceneNode* node = parentNode->createChildSceneNode();

  cube->getSubEntity(0)->setMaterial(material);
  node->attachObject(cube);
  node->setPosition(
      Ogre::Vector3(
          -element->geometry().left()*scale,
          -element->geometry().top()*scale,
          count));
  node->setScale(width, height, 1.0);
}

void DivBoxGenerator::makeElementBoxes(
    QWebElement * document,
    Ogre::Real scale,
    Ogre::Real step,
    QString tagName,
    Ogre::String meshName,
    Ogre::SceneManager * sceneManager) {

  QWebElementCollection elements = document->findAll(tagName);
  Ogre::Real count = 0;

  foreach(QWebElement element, elements) {
      if (fits(&element, 0, 2048)) {
        qDebug() << "Some " << tagName << " " << element.geometry();
        Ogre::Entity* cube = sceneManager->createEntity(meshName);
        Ogre::String textureName =
            "PageTex" + Ogre::StringConverter::toString(count);

        makeOgreImage(&element, textureName);
        attachNode(&element, sceneManager->getRootSceneNode(), scale, count,
            textureName, cube);

        count += step;
      }
    }
}

bool DivBoxGenerator::fits(QWebElement * element, unsigned min, unsigned max) {
  if (element->geometry().width() > min &&
      element->geometry().height() > min &&
      element->geometry().width() < max &&
      element->geometry().height() < max)
    return true;
  return false;
}

Level* DivBoxGenerator::generate(Ogre::SceneManager * sceneManager) {
  QWebElement document = webpage->mainFrame()->documentElement();
//  QSize siteResolution = document.geometry().size();
  QSize siteResolution = QSize(1024, 1024);
  qDebug() << "Whole Page " << webpage->mainFrame()->geometry();

  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Vertical, Qt::ScrollBarAlwaysOff);
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  webpage->setViewportSize(siteResolution);

  makeElementBoxes(&document, 0.01, 2, "img", "Cube.mesh", sceneManager);

  sceneManager->createLight("Light")->setPosition(75, 75, 75);

  return new Level();
}
