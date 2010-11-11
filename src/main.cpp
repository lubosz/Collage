#include <stdio.h>
#include <string>
#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>

using namespace std;

    Ogre::RenderWindow * window;
    Ogre::SceneManager * scene;
    Ogre::Root * root;

int main(int argc, char *argv[])
{
	string pluginsPath = "";
	string resourcePath = "res/";
	root = new Ogre::Root(pluginsPath, resourcePath + "ogre.cfg", resourcePath + "ogre.log");
	if (root->showConfigDialog())
		window = root->initialise(true);
	root->startRendering();
}
