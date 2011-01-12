/*
 *  Copyright 2010 The Collage Project
 */
#include "LevelGeneratorManager.h"
#include <iostream>

LevelGeneratorManager::LevelGeneratorManager(QObject *parent) :
    QObject(parent)
{
    connect(&webpage, SIGNAL(loadProgress(int)),
		this, SLOT(printProgress(int)));
    connect(&webpage, SIGNAL(loadFinished(bool)),
		this, SLOT(getMatchingGenerator(bool)));
//    webpage.mainFrame()->load(QUrl("http://www.example.net"));
    //this->requestLock = false;
    // Add all the different generators to our list of generators,
    // most general LAST!
//    this->addGenerator(new GeneralLevelGenerator());
}

void LevelGeneratorManager::addGenerator(LevelGenerator *generator) {
    this->generators.push_back(generator);
}

void LevelGeneratorManager::requestWebpage(QString _url) {
    QUrl *url = new QUrl(_url);
//    if (this->requestLock) {
//        qDebug() << "Webpage request already in progress.";
//        return;
//    }
    qDebug() << "Loading" << *url;
//    this->requestLock = true;

//    this->percent = 0;
    webpage.mainFrame()->load(*url);
    webpage.mainFrame()->setScrollBarPolicy(
		Qt::Vertical, Qt::ScrollBarAlwaysOff);
    webpage.mainFrame()->setScrollBarPolicy(
		Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    webpage.setViewportSize(QSize(1024, 768));
}

void LevelGeneratorManager::printProgress(int percent) {
//    if (this->percent >= percent)
//        return;

//    while (this->percent++ < percent)
//        std::cout << "@" << std::flush;
    qDebug() << "yay";
}

void LevelGeneratorManager::getMatchingGenerator(bool ok) {
    std::cout << "FUFUFUFUFUFUUFU done" << std::endl;
    if (!ok) {
        qDebug() << "Request failed.";
        // TODO(dennda):
        // ERROR HANDLING!
    }
    qDebug() << "Finished loading webpage";
//    qDebug() << this->webpage.viewportSize();
//    LevelGenerator *gen = NULL, *best_gen = NULL;
//    float score, best_score = MAX_SCORE + 1.;
//    foreach(gen, this->generators) {
//        score = gen->getScore(&this->webpage);
//        if (score < best_score) {
//            best_score = score;
//            best_gen = gen;
//        }
//    }
//    if (best_gen == NULL) {
//        qDebug() << "No suitable generator found.";
//        return;
//    }

//    // No need to pass the webpage, the generator still has it from getScore()
//    Level *level = best_gen->generate();
    Level *level = NULL;
    emit levelGenerated(level);
//    this->requestLock = false;
}
