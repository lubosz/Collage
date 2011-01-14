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
  this->name = "Deine Mudder";
}

float DivBoxGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 10.0;
}

Level* DivBoxGenerator::generate(Ogre::SceneManager * sceneManager) {
  QWebElement document = webpage->mainFrame()->documentElement();
//  QSize siteResolution = document.geometry().size();
  QSize siteResolution = QSize(1024, 1024);
  qDebug() << "Whole Page " << webpage->mainFrame()->geometry();
  std::string targetImage = "foo";

  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Vertical, Qt::ScrollBarAlwaysOff);
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  webpage->setViewportSize(siteResolution);
  QImage image(siteResolution, QImage::Format_ARGB32_Premultiplied);

  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  webpage->mainFrame()->documentElement().render(&painter);
  painter.end();

  Ogre::TextureManager::getSingleton().remove(targetImage);
  Ogre::TextureManager::getSingleton().loadImage(
      targetImage,
      "General",
      Ogre::Image().loadDynamicImage(
          image.bits(),
          image.width(),
          image.height(),
          1,
          Ogre::PF_A8R8G8B8));


  Ogre::SceneNode* node;
  Ogre::Entity* cube;
  Ogre::MaterialPtr cubeMat;

  cubeMat = Ogre::MaterialManager::getSingleton().create(
        "PageMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  cubeMat.get()->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
  cubeMat.get()->getTechnique(0)->
      getPass(0)->createTextureUnitState(targetImage);

  QWebElementCollection elements = document.findAll("div");
  Ogre::Real count = 0;
  Ogre::Real scale = .01;

  Ogre::Real width = document.geometry().width()*scale;
       Ogre::Real height = document.geometry().height()*scale;

       node = sceneManager->getRootSceneNode()->createChildSceneNode();
       cube = sceneManager->createEntity("Cube.mesh");
       cube->getSubEntity(0)->setMaterial(cubeMat);
       node->attachObject(cube);
       node->setPosition(
           Ogre::Vector3(
               -document.geometry().left()*scale,
               -document.geometry().top()*scale,
               count));
       node->setScale(width, height, 1.0);
       count+=2;



  foreach(QWebElement element, elements) {
    if (element.geometry().width() != 0 && element.geometry().height() != 0) {
      qDebug() << "Some Div " << element.geometry();

      Ogre::Real width = element.geometry().width()*scale;
      Ogre::Real height = element.geometry().height()*scale;

      node = sceneManager->getRootSceneNode()->createChildSceneNode();
      cube = sceneManager->createEntity("Cube.mesh");
      cube->getSubEntity(0)->setMaterial(cubeMat);
      node->attachObject(cube);
      node->setPosition(
          Ogre::Vector3(
              -element.geometry().left()*scale,
              -element.geometry().top()*scale,
              count));
      node->setScale(width, height, 1.0);
      count+=2;
    }
  }

  sceneManager->createLight("Light")->setPosition(75, 75, 75);

  Level *level = new Level();
  return level;
}
