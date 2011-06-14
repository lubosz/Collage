/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include <QWebElement>
#include <QWebPage>
#include <QPainter>
#include "System.h"
#include "DivBoxLevel.h"
#include "DotSceneLoader.h"
#include "Animation.h"
#include "RenderEngine.h"

DivBoxLevel::DivBoxLevel(Ogre::SceneManager *sceneManager)
  : Level(sceneManager) {
  this->name = "DivBoxLevel";
}

float DivBoxLevel::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 4.0;
}

void DivBoxLevel::makeOgreImage(QWebElement * element,
    const Ogre::String & textureName, unsigned faces) {
  QSize renderSize = QSize(2048, 2048);
  if (element->geometry().size().width() != 0 &&
		  element->geometry().size().height() != 0)
    renderSize = element->geometry().size();
  else
	  System::Instance().logMessage("WARNING! The element Size is 0");

  QImage image(renderSize, QImage::Format_ARGB32_Premultiplied);
  image.fill(Qt::white);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setRenderHint(QPainter::TextAntialiasing, true);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  qDebug() << element->tagName() << element->geometry().size();
  element->render(&painter);
  painter.end();

//  image.save(QString::fromStdString(
//          "/home/bmonkey/Desktop/collage/"+textureName+".png"));

  Ogre::TextureManager::getSingleton().remove(textureName);
  Ogre::TextureManager::getSingleton().loadImage(textureName, "General",
      Ogre::Image().loadDynamicImage(image.bits(), image.width(),
          image.height(), faces, Ogre::PF_A8R8G8B8));
}

Ogre::MaterialPtr DivBoxLevel::makeMaterial(
    Ogre::String name, Ogre::String textureName, float intensity) {
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
      name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  material.get()->getTechnique(0)-> getPass(0)->createTextureUnitState(
      textureName);
  material.get()->getTechnique(0)-> getPass(0)->setAmbient(
      intensity, intensity, intensity);
  return material;
}

Ogre::Vector3 DivBoxLevel::attachNode(
    QWebElement * element,
    Ogre::SceneNode * node,
    Ogre::Real scale,
    const Ogre::String & textureName,
    Ogre::Entity* cube,
    Ogre::Vector3 position) {

  Ogre::MaterialPtr material =
      makeMaterial("PageMat" + Ogre::StringConverter::toString(position),
      textureName, 1.3);
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

void DivBoxLevel::makeElementBoxes(
    const QWebElement &document,
    Ogre::Real scale,
    Ogre::Real step,
    std::vector<QString> tags,
    Ogre::String meshName,
    Ogre::SceneManager * sceneManager) {

  DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
  pDotSceneLoader->parseDotScene("papercraft_man_line_running.scene",
      "General", sceneManager, sceneManager->getRootSceneNode());
  delete pDotSceneLoader;

  Animation::Instance().activate(sceneManager, "arm_left");
  Animation::Instance().activate(sceneManager, "arm_right");
  Animation::Instance().activate(sceneManager, "chest");
  Animation::Instance().activate(sceneManager, "leg_left");
  Animation::Instance().activate(sceneManager, "leg_right");
  Animation::Instance().activate(sceneManager, "pants");

  QWebElementCollection elements;
  foreach(QString tag, tags)
    elements.append(document.findAll(tag));
  Ogre::Vector3 position = Ogre::Vector3::ZERO;
  int i = 0;

  foreach(QWebElement element, elements) {
      if (fits(&element, 10, 4096)) {
//        qDebug() << "Some " << tagName << " " << element.geometry();
        Ogre::Entity* cube = sceneManager->createEntity(meshName);
        Ogre::String textureName =
            "PageTex"  + element.tagName().toStdString()
            + Ogre::StringConverter::toString(i);

//  qDebug() << "\n\nStyle" << webpage->mainFrame()->documentElement().
//      styleProperty("#background-color", QWebElement::ComputedStyle);
        element.setStyleProperty("background-color", "white");

        Ogre::Real width = element.geometry().width()*scale;
        Ogre::Real height = element.geometry().height()*scale;

        Ogre::SceneNode * boxNode = (Ogre::SceneNode*) sceneManager->
            getRootSceneNode()->createChild(textureName);
//        Actor *actor = new Actor();
        makeOgreImage(&element, textureName);

        position +=
            attachNode(&element, boxNode, scale,
            textureName, cube, position);

        Ogre::Vector3 current = simulation->terrainFactory->createActor()
           ->addPoint(-width, -height)
           ->addPoint(-width, height)
           ->addPoint(width, height)
           ->addPoint(width, -height)
           ->addPoint(-width, -height)
           ->createCollisionShape(CollisionShape2::DEF_LINESTRIP)
           ->teleport(position.x, position.y)
           ->sceneNode->_getDerivedPosition();

		if (i <= this->doors.size()) {
		  Door* door = this->doors[i];

          Ogre::SceneNode* child = door->sceneNode->createChildSceneNode();

          Ogre::Entity* doorEntity = sceneManager->createEntity("door.mesh");
          //door->sceneNode->attachObject(doorEntity);
          //door->sceneNode->setOrientation(
		  //  Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
          //door->sceneNode->setScale(20, 30, 20);

          child->attachObject(doorEntity);
          child->setOrientation(
		    Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
          child->setScale(20, 30, 20);
		  child->translate(0, 0, -5);

		  QRect geom = this->doors[i]->geometry;
		  door
		    ->addPoint(0, 0)
		    ->addPoint(geom.width(), 0)
		    ->addPoint(0, geom.height())
		    ->addPoint(geom.width(), geom.height())
		    ->createCollisionShape(CollisionShape2::DEF_AABB)
		    ->teleport(position.x - geom.width()/2., position.y + height);
		}

        if (i == 3) {
          characterSceneNode = simulation->characterFactory->createActor()
              ->addPoint(2.0, 0.0)
              ->addPoint(-2.0, 0.0)
              ->addPoint(4.0, -3.0)
              ->addPoint(-4.0, -3.0)
              ->addPoint(0.0, -10.0)
              ->createCollisionShape(CollisionShape2::DEF_CONVEX)
              ->teleport(current.x, current.y + height + 50.0)
              ->sceneNode;

          Ogre::SceneNode* sn = sceneManager->getSceneNode("Armature");
          sceneManager->getRootSceneNode()->removeChild(sn);
          characterSceneNode->addChild(sn);
          sn->scale(0.4, 0.4, 0.4);
          sn->translate(0, 10, 0);
          sn->setOrientation(
              Ogre::Quaternion(Ogre::Degree(90.0), Ogre::Vector3::UNIT_Y));
        }

        i++;
      }
    }
}

bool DivBoxLevel::fits(QWebElement * element, int min, int max) {
  if (element->geometry().width() > min &&
      element->geometry().height() > min &&
      element->geometry().width() < max &&
      element->geometry().height() < max)
    return true;
  else
    return false;
}

void DivBoxLevel::setPageRendering(const QSize& siteResolution) {
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Vertical, Qt::ScrollBarAlwaysOff);
  webpage->mainFrame()->setScrollBarPolicy(
      Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  webpage->setViewportSize(siteResolution);
}

void DivBoxLevel::generate() {
  this->sceneManager = sceneManager;

//  QSize siteResolution = document.geometry().size();
//  qDebug() << "Whole Page " << webpage->mainFrame()->geometry();
  setPageRendering(QSize(1440, 800));

  QWebElement page = webpage->mainFrame()->documentElement();
  makeOgreImage(&page, "skytex", 1);
  makeMaterial("skydome", "skytex", 0.7);
//  sceneManager->setSkyBox(true, "skyBox");
  sceneManager->setSkyDome(true, "skydome", 50, 2);

  std::vector<QString> tags;
  tags.push_back("div");
  tags.push_back("p");
  tags.push_back("img");
  tags.push_back("h2");
  tags.push_back("h1");
  tags.push_back("h3");
  tags.push_back("table");

  this->generateDoors();
  makeElementBoxes(
      page, .1, 1, tags,
      "Cube.mesh", sceneManager);
}

