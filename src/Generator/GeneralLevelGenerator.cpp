#include "GeneralLevelGenerator.h"

GeneralLevelGenerator::GeneralLevelGenerator(QObject *parent) :
    LevelGenerator(parent)
{
}

float GeneralLevelGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 1.0;
}

Level* GeneralLevelGenerator::generate() {
    // uses this.webframe to generate Level, returns Level
    Level* level;
    return level;
}
