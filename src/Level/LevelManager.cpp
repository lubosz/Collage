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
#include "LevelManager.h"

LevelManager::LevelManager(QObject *parent)
:
	QObject(parent) {
}

LevelManager::LevelManager(Ogre::SceneManager *scMgr) {
	connect(&webpage, SIGNAL(loadProgress(int)),
			this, SLOT(printProgress(int)));
	connect(&webpage, SIGNAL(loadFinished(bool)),
			this, SLOT(getMatchingLevel(bool)));
	this->requestLock = false;
	// Add all the different levels to our list,
	// most general LAST!
	this->addLevel(new TagNestingToTerrainLevel(scMgr));
	this->addLevel(new DivBoxLevel(scMgr));
	this->addLevel(new GeneralLevel(scMgr));
}

void LevelManager::addLevel(Level *level) {
	this->levels.push_back(level);
}

bool LevelManager::waitForSignal(
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

void LevelManager::sceneFromUrl(
    QString _url, Ogre::SceneManager *sceneManager) {
  this->sceneManager = sceneManager;
  requestWebpage(_url);
}

void LevelManager::requestWebpage(QString _url) {
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
  webpage.settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
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

void LevelManager::printProgress(int percent) {
	if (this->percent >= percent)
		return;

	while (this->percent++ < percent)
		std::cout << "@" << std::flush;
}

void LevelManager::getMatchingLevel(bool ok) {
	std::cout << std::endl;
	if (!ok) {
		qDebug() << "Request failed.";
		// TODO(dennda)
		// ERROR HANDLING!
	}
	qDebug() << "Finished loading webpage";
	qDebug() << this->webpage.viewportSize();
	Level *lev = NULL, *best_lev = NULL;
	float score, best_score = MAX_SCORE + 1.;
	foreach(lev, this->levels) {
		score = lev->getScore(&this->webpage);
		if (score < best_score) {
			best_score = score;
			best_lev = lev;
		}
	}
	if (best_lev == NULL) {
		qDebug() << "No suitable level found.";
		return;
	}	else {
		qDebug() << "Found Level " <<
		    best_lev->getName() << " with score " << best_score;
	}

	// No need to pass the webpage, the level still has it from getScore()
	best_lev->generate();
	emit levelGenerated(best_lev);
	this->requestLock = false;
}
