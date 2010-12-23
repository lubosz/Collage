#include <QDebug>
#include "GeneralLevelGenerator.h"

#include "RenderEngine.h"

GeneralLevelGenerator::GeneralLevelGenerator(QObject *parent) :
    LevelGenerator(parent)
{
}

float GeneralLevelGenerator::getScore(QWebPage *webpage) {
    this->webpage = webpage;
    return 100.0;
}

Level* GeneralLevelGenerator::generate() {
    // uses this.webframe to generate Level, returns Level
    Ogre::SceneManager* manager =
            RenderEngine::Instance().m_pRoot->createSceneManager(
            Ogre::ST_GENERIC, "GameSceneMgr");
    Simulation *simulation = new Simulation(manager->getRootSceneNode());

    manager->createLight("Light")->setPosition(75, 75, 75);

	simulation->createActor("myActor1", AB_STATIC, Ogre::Vector2(0.0,0.0));
	simulation->createActor("myActor2", AB_STATIC, Ogre::Vector2(1.5,0.0));
	simulation->createActor("myActor3", AB_STATIC, Ogre::Vector2(0.0,1.5));
	simulation->createActor("myActor4", AB_DYNAMIC, Ogre::Vector2(1.5,1.5));

    Level *level = new Level(simulation);
    return level;
}
