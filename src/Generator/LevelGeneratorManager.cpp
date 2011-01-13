/*
 *  Copyright 2010 The Collage Project
 */
#include <iostream>
#include <QtWebKit>
#include <QString>
#include <vector>
#include <QSignalSpy>
#include <QtCore>

#include "System.h"
#include "LevelGeneratorManager.h"

LevelGeneratorManager::LevelGeneratorManager(QObject *parent)
:
	QObject(parent) {
	connect(&webpage, SIGNAL(loadProgress(int)),
			this, SLOT(printProgress(int)));
	connect(&webpage, SIGNAL(loadFinished(bool)),
			this, SLOT(getMatchingGenerator(bool)));
	this->requestLock = false;
	// Add all the different generators to our list of generators,
	// most general LAST!
	this->addGenerator(new TagNestingToTerrainGenerator());
	this->addGenerator(new DivBoxGenerator());
	this->addGenerator(new GeneralLevelGenerator());
}

void LevelGeneratorManager::addGenerator(LevelGenerator *generator) {
	this->generators.push_back(generator);
}

bool LevelGeneratorManager::waitForSignal(
				QObject* obj, const char* signal, int timeout) {
	QEventLoop loop;
	QObject::connect(obj, signal, &loop, SLOT(quit()));
	QTimer timer;
	QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
	if (timeout > 0) {
		QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
		timer.setSingleShot(true);
		timer.start(timeout);
	}
	loop.exec();
	return timeoutSpy.isEmpty();
}

void LevelGeneratorManager::sceneFromUrl(QString _url, Ogre::SceneManager * sceneManager) {
  this->sceneManager = sceneManager;
  requestWebpage(_url);
}

void LevelGeneratorManager::requestWebpage(QString _url) {
	QUrl url(_url);
	if (this->requestLock) {
		qDebug() << "Webpage request already in progress.";
		return;
	}
	qDebug() << "Loading" << url;
	this->requestLock = true;

	this->percent = 0;

  webpage.settings()->setAttribute(QWebSettings::AutoLoadImages, true);
  webpage.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
  webpage.settings()->setAttribute(QWebSettings::JavaEnabled, false);
  webpage.settings()->setAttribute(QWebSettings::PluginsEnabled, false);
  //webpage.settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
  webpage.settings()->setAttribute(
      QWebSettings::JavascriptCanOpenWindows, false);
	webpage.mainFrame()->load(url);
	waitForSignal(webpage.mainFrame(), SIGNAL(loadFinished(bool)), 10000);
	webpage.mainFrame()->setScrollBarPolicy(
			Qt::Vertical, Qt::ScrollBarAlwaysOff);
	webpage.mainFrame()->setScrollBarPolicy(
			Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webpage.setViewportSize(QSize(1024, 768));
}

void LevelGeneratorManager::printProgress(int percent) {
	if (this->percent >= percent)
		return;

	while (this->percent++ < percent)
		std::cout << "@" << std::flush;
}

void LevelGeneratorManager::getMatchingGenerator(bool ok) {
	std::cout << std::endl;
	if (!ok) {
		qDebug() << "Request failed.";
		// TODO(dennda)
		// ERROR HANDLING!
	}
	qDebug() << "Finished loading webpage";
	qDebug() << this->webpage.viewportSize();
	LevelGenerator *gen = NULL, *best_gen = NULL;
	float score, best_score = MAX_SCORE + 1.;
	foreach(gen, this->generators) {
		score = gen->getScore(&this->webpage);
		if (score < best_score) {
			best_score = score;
			best_gen = gen;
		}
	}
	if (best_gen == NULL) {
		qDebug() << "No suitable generator found.";
		return;
	}
	else {
		qDebug() << "Found generator " << best_gen->getName() << " with score " << best_score;
	}

	// No need to pass the webpage, the generator still has it from getScore()
	Level *level = best_gen->generate(sceneManager);
	emit levelGenerated(level);
	this->requestLock = false;
}
