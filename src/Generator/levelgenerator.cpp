#include "levelgenerator.h"

LevelGenerator::LevelGenerator(QObject *parent) :
    QObject(parent)
{
}

float LevelGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 1.0;
}

Level* LevelGenerator::generate() {
    // uses this.webframe to generate Level, returns Level
    Level* level;
    return level;
}
