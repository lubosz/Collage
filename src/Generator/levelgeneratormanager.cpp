#include "levelgeneratormanager.h"

LevelGeneratorManager::LevelGeneratorManager(QObject *parent) :
    QObject(parent)
{

}

void LevelGeneratorManager::addGenerator(LevelGenerator *generator) {
    this->generators.push_back(generator);
}

LevelGenerator* LevelGeneratorManager::getMatchingGenerator(QUrl url) {
    // TODO: get frame from url
    QWebFrame *frame;
    LevelGenerator *gen, *best_gen;
    float score, best_score;
    foreach(gen, this->generators) {
        score = gen->getScore(frame);
        if (score < best_score) {
            best_score = score;
            best_gen = gen;
        }
    }
    return best_gen;
}
