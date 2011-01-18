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
  image.fill(Qt::white);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  element->render(&painter);
  painter.end();

//  image.save(QString::fromStdString(
//          "/home/bmonkey/Desktop/collage/"+textureName+".png"));

  Ogre::TextureManager::getSingleton().remove(textureName);
  Ogre::TextureManager::getSingleton().loadImage(textureName, "General",
      Ogre::Image().loadDynamicImage(image.bits(), image.width(),
          image.height(), 1, Ogre::PF_A8R8G8B8));
}

Ogre::Vector3 DivBoxGenerator::attachNode(
    QWebElement * element,
    Ogre::SceneNode * node,
    Ogre::Real scale,
    const Ogre::String & textureName,
    Ogre::Entity* cube,
    Ogre::Vector3 position) {

  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
      "PageMat" + Ogre::StringConverter::toString(position),
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  material.get()->getTechnique(0)-> getPass(0)->createTextureUnitState(
      textureName);

  material.get()->setDiffuse(1, 1, 1, 1);

//  material.get()->setSceneBlending(
//      Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_DEST_ALPHA);
//  material.get()->getTechnique(0)->getPass(0)->setSceneBlending(
//      Ogre::SBT_TRANSPARENT_COLOUR);

  Ogre::Real width = element->geometry().width()*scale;
  Ogre::Real height = element->geometry().height()*scale;

  cube->getSubEntity(1)->setMaterial(material);
  Ogre::Real x, y, z;
  x = width*2;
  y = -height/3.0;
  z = 0;

  Ogre::Vector3 move(x, y, z);

  qDebug() << "Position:" << x << y << z
      << "Size:" << width << height << "Scale:" << scale;
  node->attachObject(cube);
  node->setPosition(move + position);
  node->setScale(width, height, width);

  return move;
}

void DivBoxGenerator::makeElementBoxes(
    const QWebElement &document,
    Ogre::Real scale,
    Ogre::Real step,
    std::vector<QString> tags,
    Ogre::String meshName,
    Ogre::SceneManager * sceneManager) {

  QWebElementCollection elements;
  foreach(QString tag, tags)
    elements.append(document.findAll(tag));
  Ogre::Vector3 position = Ogre::Vector3();
  int i = 0;

  foreach(QWebElement element, elements) {
      if (fits(&element, 0, 4096)) {
//        qDebug() << "Some " << tagName << " " << element.geometry();
        Ogre::Entity* cube = sceneManager->createEntity(meshName);
        Ogre::String textureName =
            "PageTex"  + element.tagName().toStdString()
            + Ogre::StringConverter::toString(i);

        element.setStyleProperty("background-color", "white");

        Ogre::Real width = element.geometry().width()*scale;
        Ogre::Real height = element.geometry().height()*scale;

        Actor *actor = this->simulation->createActor(IT_TERRAIN, CT_AABB);
        CSAABB *aabb = static_cast<CSAABB*>(actor->getCollisionShape());
        aabb->aABB.x = width;
        aabb->aABB.y = height;
        makeOgreImage(&element, textureName);
        position +=
            attachNode(&element, actor->getSceneNode(), scale,
            textureName, cube, position);
        i++;
      }
    }
}

bool DivBoxGenerator::fits(QWebElement * element, int min, int max) {
  if (element->geometry().width() > min &&
      element->geometry().height() > min &&
      element->geometry().width() < max &&
      element->geometry().height() < max)
    return true;
  else
    return false;
}

void DivBoxGenerator::setPageRendering(const QSize& siteResolution) {
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Vertical, Qt::ScrollBarAlwaysOff);
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  webpage->setViewportSize(siteResolution);
}

Level* DivBoxGenerator::generate(Ogre::SceneManager *sceneManager,
    Simulation *simulation) {
  this->sceneManager = sceneManager;
  this->simulation = simulation;
  qDebug() << "\n\nStyle" << webpage->mainFrame()->documentElement().
      styleProperty("#background-color", QWebElement::ComputedStyle);

//  QSize siteResolution = document.geometry().size();
//  qDebug() << "Whole Page " << webpage->mainFrame()->geometry();
  setPageRendering(QSize(1440, 800));

  std::vector<QString> tags = {"div", "p", "img", "h2", "h1", "h3", "table"};

  makeElementBoxes(
      webpage->mainFrame()->documentElement(),
      .01, 1, tags,
      "Cube.mesh", sceneManager);

  sceneManager->createLight("Light")->setPosition(75, 75, 75);

  this->addDoors();

  return new Level();
}
