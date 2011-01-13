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
	return 100.0;
}

void TagNestingToTerrainGenerator::addBox(unsigned height) {
  for (unsigned i = 0; i < height; i++)
    std::cout << "." << std::flush;
  std::cout << std::endl;
}

Level* TagNestingToTerrainGenerator::generate(
    Ogre::SceneManager * sceneManager) {
	qDebug() << "Beginning generation... TagNestingToTerrainGenerator";
	QWebFrame *frame = this->webpage->mainFrame();
	QWebElement document = frame->documentElement();

	QWebElement cur = document.findFirst("body");
	unsigned indent = 0;

  QWebElement last = cur;
	while (!cur.isNull()) {
		cur = cur.firstChild();
    indent++;
		if (!cur.isNull()) {
      last = cur;
      this->addBox(indent);
		}	else {
      QWebElement sibling = last.nextSibling();
      while (sibling.isNull() && (last.tagName() != "BODY")) {
        last = last.parent();
        sibling = last.nextSibling();
        indent--;
        this->addBox(indent);
		  }
      if (!sibling.isNull()) {
        cur = sibling;
        last = sibling;
        indent--;
        this->addBox(indent);
      }
	  }
  }

	Level *level = NULL;  // new Level(simulation);
	return level;
}
