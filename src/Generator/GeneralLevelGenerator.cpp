/*
 *  Copyright 2010 The Collage Project
 */
#include <QDebug>
#include "GeneralLevelGenerator.h"

GeneralLevelGenerator::GeneralLevelGenerator(QObject *parent)
:
    LevelGenerator(parent) {
}

float GeneralLevelGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 100.0;
}

Level* GeneralLevelGenerator::generate() {
    // uses this.webframe to generate Level, returns Level
    Level* level;
    qDebug() << "YAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    return level;
}
