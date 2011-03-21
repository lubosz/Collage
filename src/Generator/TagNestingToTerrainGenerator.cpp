/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include "TagNestingToTerrainGenerator.h"

#include "RenderEngine.h"

TagNestingToTerrainGenerator::TagNestingToTerrainGenerator(QObject *parent)
	:	LevelGenerator(parent) {
			this->name = "TagNestingToTerrainGenerator";
		}

float TagNestingToTerrainGenerator::getScore(QWebPage *webpage) {
	this->webpage = webpage;
	return 2.0;
}

void TagNestingToTerrainGenerator::addBox(
    unsigned height, unsigned count, Ogre::ManualObject *manual) {

  // for (unsigned i = 0; i < height; i++)
  //   std::cout << "." << std::flush;
  // std::cout << std::endl;


  // 'radius', half the width
  float radz = 5.;
  float radx = 1.;
  // Front
  manual->normal(0, 1, 0);
  manual->position(count, height,  radz);
  // Back
  manual->normal(0, 1, 0);
  manual->position(count, height, -radz);
}

Level* TagNestingToTerrainGenerator::generate(
    Ogre::SceneManager *sceneManager,
    Simulation *simulation) {

  this->sceneManager = sceneManager;
  this->simulation = simulation;

  sceneManager->createLight("Light")->setPosition(75, 75, 75);
  Ogre::ManualObject *manual = sceneManager->createManualObject("ground");
  manual->begin("BaseWhiteNoLighting",
                Ogre::RenderOperation::OT_TRIANGLE_STRIP);

	qDebug() << "Beginning generation... TagNestingToTerrainGenerator";
	QWebFrame *frame = this->webpage->mainFrame();
	QWebElement document = frame->documentElement();

	QWebElement cur = document.findFirst("body");
	unsigned indent = 0;
  unsigned count = 0;

  QWebElement last = cur;
	while (!cur.isNull()) {
		cur = cur.firstChild();
    indent++;
		if (!cur.isNull()) {
      last = cur;
      this->addBox(indent, count++, manual);
		}	else {
      QWebElement sibling = last.nextSibling();
      while (sibling.isNull() && (last.tagName() != "BODY")) {
        last = last.parent();
        sibling = last.nextSibling();
        indent--;
        this->addBox(indent, count++, manual);
		  }
      if (!sibling.isNull()) {
        cur = sibling;
        last = sibling;
        indent--;
        this->addBox(indent, count++, manual);
      }
	  }
  }

  manual->end();
  manual->setMaterialName(0, "GroundNoCulling");
  sceneManager->getRootSceneNode()->
            createChildSceneNode()->attachObject(manual);

  this->addDoors();

  Level *level = new Level();
  return level;
}
