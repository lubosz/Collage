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
	return 50.0;
}

void TagNestingToTerrainGenerator::addBox(
    unsigned height, unsigned count, Ogre::SceneManager *sceneManager) {

  // for (unsigned i = 0; i < height; i++)
  //   std::cout << "." << std::flush;
  // std::cout << std::endl;

  Ogre::SceneNode* node;
  Ogre::Entity* cube;

  node = sceneManager->getRootSceneNode()->createChildSceneNode();
  cube = sceneManager->createEntity("Cube.mesh");
  node->attachObject(cube);
  node->setPosition(
      Ogre::Vector3(
          count * 6,
          height * 6,
          0));
  node->setScale(1., 1., 10.);
}

Level* TagNestingToTerrainGenerator::generate(
    Ogre::SceneManager *sceneManager) {

  sceneManager->createLight("Light")->setPosition(75, 75, 75);

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
      this->addBox(indent, count++, sceneManager);
		}	else {
      QWebElement sibling = last.nextSibling();
      while (sibling.isNull() && (last.tagName() != "BODY")) {
        last = last.parent();
        sibling = last.nextSibling();
        indent--;
        this->addBox(indent, count++, sceneManager);
		  }
      if (!sibling.isNull()) {
        cur = sibling;
        last = sibling;
        indent--;
        this->addBox(indent, count++, sceneManager);
      }
	  }
  }

  Level *level = new Level();
  return level;
}
