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
  qDebug() << "Creating " << this->name;
  characterSceneNode = NULL;
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
  Ogre::MaterialManager::getSingleton().unload(name);
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
      name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  material.get()->getTechnique(0)-> getPass(0)->createTextureUnitState(
      textureName);
  material.get()->getTechnique(0)-> getPass(0)->setAmbient(
      intensity, intensity, intensity);
  return material;
}

void DivBoxLevel::makeElementBoxes(
    const QWebElement &document,
    Ogre::Real scale,
    std::vector<QString> tags,
    Ogre::String meshName,
    Ogre::SceneManager * sceneManager) {

  DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
  pDotSceneLoader->parseDotScene("papercraft_man_line_running.scene",
      "General", sceneManager, sceneManager->getRootSceneNode());
  delete pDotSceneLoader;

  Animation::Instance().animationStates.clear();
  Animation::Instance().init(sceneManager, "arm_left");
  Animation::Instance().init(sceneManager, "arm_right");
  Animation::Instance().init(sceneManager, "chest");
  Animation::Instance().init(sceneManager, "leg_left");
  Animation::Instance().init(sceneManager, "leg_right");
  Animation::Instance().init(sceneManager, "pants");

  QWebElementCollection elements;
  foreach(QString tag, tags)
    elements.append(document.findAll(tag));

  Ogre::Vector3 position = Ogre::Vector3(0.0, 100.0, 0.0);

  int elementCount = 0;
  foreach(QWebElement element, elements) {
      if (fits(&element, 10, 4096)) {
        Ogre::String textureName =
            "PageTex"  + element.tagName().toStdString()
            + Ogre::StringConverter::toString(elementCount);

        Box box;
        box.width = element.geometry().width()*scale;
        box.height = element.geometry().height()*scale;

        box.sceneNode = sceneManager->createSceneNode(textureName);

        element.setStyleProperty("background-color", "white");

        Ogre::Entity* cube = sceneManager->createEntity(meshName);

        makeOgreImage(&element, textureName);
        Ogre::MaterialPtr material =
            makeMaterial("PageMat" + Ogre::StringConverter::toString(position),
            textureName, 1.3);
        cube->getSubEntity(1)->setMaterial(material);
        box.sceneNode->attachObject(cube);
        box.sceneNode->setScale(box.width, box.height, box.width);

        if (box.width > 50)
          bigBoxes.push_back(box);
        else
          smallBoxes.push_back(box);

        elementCount++;
      }
  }

  int smallBoxIndex = 0;

  for (int i = 0; i < bigBoxes.size(); i++) {
    CollisionActor* actor = simulation->terrainFactory->createActor()
       ->addPoint(bigBoxes[i].width, bigBoxes[i].height)
       ->addPoint(-bigBoxes[i].width, -bigBoxes[i].height)
       ->createCollisionShape(CollisionShape2::DEF_AABB);

    if (i == 0) {
// FIRST PLANE-----------------------
      actor->teleport(100.0, 100.0);

      characterSceneNode = simulation->characterFactory->createActor()
          ->addPoint(4.0, 30.0)
          ->addPoint(-4.0, -10.0)
          ->createCollisionShape(CollisionShape2::DEF_CONVEX)
          ->teleport(100.0, bigBoxes[i].height * 2 + 200.0)
          ->sceneNode;

      Ogre::SceneNode* sn = sceneManager->getSceneNode("Armature");
      sceneManager->getRootSceneNode()->removeChild(sn);
      characterSceneNode->addChild(sn);
      sn->scale(0.4, 0.4, 0.4);
      sn->translate(0, 10, 0);
      sn->setOrientation(
          Ogre::Quaternion(Ogre::Degree(90.0), Ogre::Vector3::UNIT_Y));

    } else {
// FURTHER PLANES--------------------
      float lX = bigBoxes[i - 1].sceneNode->_getDerivedPosition().x;
      float lY = bigBoxes[i - 1].sceneNode->_getDerivedPosition().y;
      float lW = bigBoxes[i - 1].width;
      float lH = bigBoxes[i - 1].height;
      float cX = bigBoxes[i].sceneNode->_getDerivedPosition().x;
      float cY = bigBoxes[i].sceneNode->_getDerivedPosition().y;
      float cW = bigBoxes[i].width;
      float cH = bigBoxes[i].height;

      if (cH - lH > 50.0 && smallBoxes.size() - 1 - smallBoxIndex > 1) {
        // current is mutch higher then last
        actor->teleport(lX + lW + cW, 100.0);

        float sX = smallBoxes[smallBoxIndex].sceneNode->_getDerivedPosition().x;
        float sY = smallBoxes[smallBoxIndex].sceneNode->_getDerivedPosition().y;
        float sW = smallBoxes[smallBoxIndex].width;
        float sH = smallBoxes[smallBoxIndex].height;
        CollisionActor* hoverplane = simulation->hoverplaneFactory
            ->createActor()
            ->addPoint(sW, sH)
            ->addPoint(-sW, -sH)
            ->createCollisionShape(CollisionShape2::DEF_AABB);
        static_cast<Hoverplane*>(hoverplane)
                      ->setSpeed(0.2 + static_cast<float>
                  (static_cast<int>(lX * cX * cW) % 5) * 0.1)
            ->setPath(lX + lW - sW - 100.0, lY + lH - sH + 10.0,
                lX + lW - sW - 100.0, lY + lH - sH + cH - lH);
        hoverplane->sceneNode->addChild(smallBoxes[smallBoxIndex].sceneNode);
        smallBoxIndex++;
      } else {
        if (lH - cH > 50.0 && smallBoxes.size() - 1 - smallBoxIndex > 1) {
          actor->teleport(lX + lW + cW, 100.0);

          float sX = smallBoxes[smallBoxIndex].sceneNode->
              _getDerivedPosition().x;
          float sY = smallBoxes[smallBoxIndex].sceneNode->
              _getDerivedPosition().y;
          float sW = smallBoxes[smallBoxIndex].width;
          float sH = smallBoxes[smallBoxIndex].height;

          CollisionActor* hoverplane = simulation->hoverplaneFactory
              ->createActor()
              ->addPoint(sW, sH)
              ->addPoint(-sW, -sH)
              ->createCollisionShape(CollisionShape2::DEF_AABB);
          static_cast<Hoverplane*>(hoverplane)
                      ->setSpeed(0.2 + static_cast<float>
                  (static_cast<int>(lX * cX * cW) % 5) * 0.1)
              ->setPath(cX, cY + cH + sH + 10.0,
                  cX, cY + cH + sH + lH);
          hoverplane->sceneNode->addChild(smallBoxes[smallBoxIndex].sceneNode);
          smallBoxIndex++;
        } else {
          if (i%3 == 2 && smallBoxes.size() - 1 - smallBoxIndex > 3) {
            actor->teleport(lX + lW + cW +
                smallBoxes[smallBoxIndex].sceneNode
                ->_getDerivedPosition().x +
                smallBoxes[smallBoxIndex+1].sceneNode
                ->_getDerivedPosition().x +
                smallBoxes[smallBoxIndex+2].sceneNode
                ->_getDerivedPosition().x
                + 400.0,
                100.0);
            {
              float sX = smallBoxes[smallBoxIndex].sceneNode->
                  _getDerivedPosition().x;
              float sY = smallBoxes[smallBoxIndex].sceneNode->
                  _getDerivedPosition().y;
              float sW = smallBoxes[smallBoxIndex].width;
              float sH = smallBoxes[smallBoxIndex].height;
              CollisionActor* hoverplane = simulation->hoverplaneFactory
                  ->createActor()
                  ->addPoint(sW, sH)
                  ->addPoint(-sW, -sH)
                  ->createCollisionShape(CollisionShape2::DEF_AABB);
              static_cast<Hoverplane*>(hoverplane)
                      ->setSpeed(0.1 + static_cast<float>
                  (static_cast<int>(lX * cX * sW) % 5) * 0.1)
                  ->setPath(lX + lW + 100.0, lY + lH - sH,
                      lX + lW + 100.0, lY + lH + cH + 200.0);
              hoverplane->sceneNode
              ->addChild(smallBoxes[smallBoxIndex].sceneNode);
            }
            {
              float sX = smallBoxes[smallBoxIndex+1].sceneNode->
                  _getDerivedPosition().x;
              float sY = smallBoxes[smallBoxIndex+1].sceneNode->
                  _getDerivedPosition().y;
              float sW = smallBoxes[smallBoxIndex+1].width;
              float sH = smallBoxes[smallBoxIndex+1].height;
              CollisionActor* hoverplane = simulation->hoverplaneFactory
                  ->createActor()
                  ->addPoint(sW, sH)
                  ->addPoint(-sW, -sH)
                  ->createCollisionShape(CollisionShape2::DEF_AABB);
              static_cast<Hoverplane*>(hoverplane)
                      ->setSpeed(0.2 + static_cast<float>
                  (static_cast<int>(lX * cX * sW) % 5) * 0.1)
                  ->setPath(lX + lW + 200.0, lY + lH - sH,
                      lX + lW + 200.0, lY + lH + cH + 200.0);
              hoverplane->sceneNode
              ->addChild(smallBoxes[smallBoxIndex+1].sceneNode);
            }
            {
              float sX = smallBoxes[smallBoxIndex+2].sceneNode->
                  _getDerivedPosition().x;
              float sY = smallBoxes[smallBoxIndex+2].sceneNode->
                  _getDerivedPosition().y;
              float sW = smallBoxes[smallBoxIndex+2].width;
              float sH = smallBoxes[smallBoxIndex+2].height;
              CollisionActor* hoverplane = simulation->hoverplaneFactory
                  ->createActor()
                  ->addPoint(sW, sH)
                  ->addPoint(-sW, -sH)
                  ->createCollisionShape(CollisionShape2::DEF_AABB);
              static_cast<Hoverplane*>(hoverplane)
                  ->setSpeed(0.15 + static_cast<float>
              (static_cast<int>(lX * cX * sW) % 5) * 0.1)
                  ->setPath(lX + lW + 300.0, lY + lH - sH,
                      lX + lW + 300.0, lY + lH + cH + 200.0);
              hoverplane->sceneNode
              ->addChild(smallBoxes[smallBoxIndex+2].sceneNode);
            }

            smallBoxIndex += 3;
          } else {
            actor->teleport(lX + lW + cW + 100.0, 100.0);
          }
        }
      }
    }
    actor->sceneNode->addChild(bigBoxes[i].sceneNode);
    qDebug() << "dooors" << doors.size() << "\n\n";
    if ((i == (bigBoxes.size()-1) / 2 || i == bigBoxes.size()-1)
        && this->doors.size() >= 1) {
      CollisionActor* door;

      if (i == (bigBoxes.size()-1) / 2) {
        door = this->doors[0];
        qDebug() << "dooors0\n\n\n\n\n";
      } else {
        door = this->doors[0];
        qDebug() << "dooors1\n\n\n\n\n";
      }
      door
          ->addPoint(0.0, 0.0)
          ->addPoint(40., 60.)
          ->createCollisionShape(CollisionShape2::DEF_AABB)
          ->teleport(
              bigBoxes[i].sceneNode->_getDerivedPosition().x,
              bigBoxes[i].sceneNode->_getDerivedPosition().y
              + bigBoxes[i].height);

      Ogre::Entity* doorEntity = sceneManager->createEntity("door.mesh");
      door->sceneNode->attachObject(doorEntity);
      door->sceneNode->setScale(20, 30, 20);
      door->sceneNode->setOrientation(
              Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
    }
  }
}
//        position += Ogre::Vector3(
//            width * 2.0 + 2.0 * static_cast<float>(
//                static_cast<int>(width * height) % 1000),
//            -height/3.0, 0);
//
//        Ogre::SceneNode* currentSceneNode;
//        if (width > 40.0) {
//          currentSceneNode = simulation->terrainFactory->createActor()
//             ->addPoint(-width, -height)
//             ->addPoint(width, height)
//             ->createCollisionShape(CollisionShape2::DEF_AABB)
//             ->teleport(position.x, position.y)
//             ->sceneNode;
//        } else {
//          CollisionActor* hoverplane = simulation->hoverplaneFactory
//              ->createActor()
//              ->addPoint(-width, -height)
//              ->addPoint(width, height)
//              ->createCollisionShape(CollisionShape2::DEF_AABB)
//              ->teleport(position.x, position.y);
//          static_cast<Hoverplane*>(hoverplane)
//              ->setSpeed(0.2 +
//                  static_cast<float>(static_cast<int>(width * height) % 5)
//                  * 0.1)
//              ->setPath(position.x, position.y,
//                  position.x, position.y + 150.0 + width);
//          currentSceneNode = hoverplane->sceneNode;
//        }
//        Ogre::Vector3 current = currentSceneNode->_getDerivedPosition();
//
//        attachNode(&element, currentSceneNode, scale,
//        textureName, cube, position) + width;
//
//  	if ((i < this->doors.size()) && ((i % 3) == 0)) {
//  	  Door* door = this->doors[i];
//
//          Ogre::SceneNode* child = door->sceneNode->createChildSceneNode();
//
//          Ogre::Entity* doorEntity = sceneManager->createEntity("door.mesh");
//          // door->sceneNode->attachObject(doorEntity);
//          // door->sceneNode->setOrientation(
//  	  //   Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
//          // door->sceneNode->setScale(20, 30, 20);
//
//          child->attachObject(doorEntity);
//          child->setOrientation(
//  	    Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
//          child->setScale(20, 30, 20);
//  	  // child->translate(0, 0, -5);
//
//  	  // FIXME figure this out based on the door mesh
//  	  double w = 40;
//  	  double h = 60;
//  	  door
//  	    ->addPoint(0, 0)
//  	    ->addPoint(w, 0)
//  	    ->addPoint(0, h)
//  	    ->addPoint(w, h)
//  	    ->createCollisionShape(CollisionShape2::DEF_LINESTRIP)
//  	    ->teleport(position.x - w/2., position.y + height);
//  	}
//
//        if (i == 0) {
//          characterSceneNode = simulation->characterFactory->createActor()
//              ->addPoint(4.0, 30.0)
//              ->addPoint(-4.0, -10.0)
//              ->createCollisionShape(CollisionShape2::DEF_CONVEX)
//              ->teleport(current.x, current.y + height + 300.0)
//              ->sceneNode;
//
//          CollisionActor *hoverplane = simulation->hoverplaneFactory
//              ->createActor()
//              ->addPoint(-20.0, -2.0)
//              ->addPoint(20.0, 2.0)
//              ->createCollisionShape(CollisionShape2::DEF_AABB);
//          static_cast<Hoverplane*>(hoverplane)
//              ->setSpeed(0.5)
//              ->setPath(current.x, current.y + height + 100.0,
//                  current.x, current.y + height + 10.0);
//
//
//          Ogre::SceneNode* sn = sceneManager->getSceneNode("Armature");
//          sceneManager->getRootSceneNode()->removeChild(sn);
//          characterSceneNode->addChild(sn);
//          sn->scale(0.4, 0.4, 0.4);
//          sn->translate(0, 10, 0);
//          sn->setOrientation(
//              Ogre::Quaternion(Ogre::Degree(90.0), Ogre::Vector3::UNIT_Y));
//        }
//
//        i++;
//      }
//    }
//  }

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
      page, .1, tags,
      "Cube.mesh", sceneManager);
}

