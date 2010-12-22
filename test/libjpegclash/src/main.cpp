#include "FrameCapture.h"

#include <iostream>
#include <QtGui>

#include <OGRE/Ogre.h>

using namespace Ogre;

void initResources() {
    String secName, typeName, archName;
    ConfigFile cf;
    cf.load("ogreTest.cfg");

    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().
            		addResourceLocation(archName, typeName, secName);
        }
    }
}

void startOgre(){
	Root* m_pRoot = new Root();
    initResources();
    LogManager::getSingleton().createLog("BasicOgre.log", true, true, false);
    m_pRoot->showConfigDialog();

    RenderWindow* m_pRenderWnd = m_pRoot->initialise(true, "Basic Ogre");
    m_pRenderWnd->setActive(true);
    Viewport* m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	SceneManager* m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(ColourValue(0.7f, 0.7f, 0.7f));

	Camera* m_pCamera;
	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(5, 60, 60));
	m_pCamera->lookAt(Vector3(5, 20, 0));
	m_pCamera->setNearClipDistance(5);

	m_pCamera->setAspectRatio(
			Real(m_pViewport->getActualWidth())
			/ Real(m_pViewport->getActualHeight())
	);

	m_pViewport->setCamera(m_pCamera);

	SceneNode* m_pOgreHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
	m_pOgreHeadNode->attachObject(m_pSceneMgr->createEntity("Cube", "ogrehead.mesh"));
	m_pOgreHeadNode->setPosition(Vector3(0, 0, -25));

	while (true) {
		m_pOgreHeadNode->rotate(
				Vector3(0,1,0),
				Radian(.001)
		);
		m_pRoot->renderOneFrame();
	}
}

int main(int argc, char * argv[])
{
    QUrl url = QUrl("http://en.wikipedia.org/wiki/Special:Random");
    QString fileName = "random.png";

    QApplication a(argc, argv);
    FrameCapture capture;
    QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
    capture.load(url, fileName);




    a.exec();
    //startOgre();
}

