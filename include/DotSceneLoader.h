/*
 * AppStateManager.h
 *
 *  Created on: Nov 14, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

// Includes
#include <OGRE/OgreString.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <vector>

#include "rapidxml.hpp"

// Forward declarations
namespace Ogre {
class SceneManager;
class SceneNode;
class TerrainGroup;
class TerrainGlobalOptions;
}

namespace Forests {
class PagedGeometry;
class TreeLoader3D;
}

class nodeProperty {
 public:
	Ogre::String nodeName;
	Ogre::String propertyNm;
	Ogre::String valueName;
	Ogre::String typeName;

	nodeProperty(const Ogre::String &node, const Ogre::String &propertyName,
      const Ogre::String &value, const Ogre::String &type)
:
    nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {
  }
};

class DotSceneLoader {
 public:
#ifdef USE_OGRE_TERRAIN
	Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;
#endif

	DotSceneLoader();
	virtual ~DotSceneLoader();

	void parseDotScene(const Ogre::String &SceneName,
			const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr,
			Ogre::SceneNode *pAttachNode = NULL,
			const Ogre::String &sPrependNode = "");
	Ogre::String
			getProperty(const Ogre::String &ndNm, const Ogre::String &prop);

#ifdef USE_OGRE_TERRAIN
	Ogre::TerrainGroup* getTerrainGroup() {
		return mTerrainGroup;
	}
#endif

	std::vector<nodeProperty> nodeProperties;
	std::vector<Ogre::String> staticObjects;
	std::vector<Ogre::String> dynamicObjects;
	std::vector<Forests::PagedGeometry *> mPGHandles;
	std::vector<Forests::TreeLoader3D *> mTreeHandles;

 protected:
	void processScene(rapidxml::xml_node<>* XMLRoot);

	void processNodes(rapidxml::xml_node<>* XMLNode);
	void processExternals(rapidxml::xml_node<>* XMLNode);
	void processEnvironment(rapidxml::xml_node<>* XMLNode);
	void processTerrain(rapidxml::xml_node<>* XMLNode);
	void processTerrainPage(rapidxml::xml_node<>* XMLNode);
	void processBlendmaps(rapidxml::xml_node<>* XMLNode);
	void processUserDataReference(rapidxml::xml_node<>* XMLNode,
			Ogre::SceneNode *pParent = 0);
	void processUserDataReference(rapidxml::xml_node<>* XMLNode,
			Ogre::Entity *pEntity);
	void processOctree(rapidxml::xml_node<>* XMLNode);
	void processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent =
			0);
	void processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent =
			0);

	void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent =
			0);
	void processLookTarget(rapidxml::xml_node<>* XMLNode,
			Ogre::SceneNode *pParent);
	void processTrackTarget(rapidxml::xml_node<>* XMLNode,
			Ogre::SceneNode *pParent);
	void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processParticleSystem(rapidxml::xml_node<>* XMLNode,
			Ogre::SceneNode *pParent);
	void processBillboardSet(rapidxml::xml_node<>* XMLNode,
			Ogre::SceneNode *pParent);
	void processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processPagedGeometry(rapidxml::xml_node<>* XMLNode);

	void processFog(rapidxml::xml_node<>* XMLNode);
	void processSkyBox(rapidxml::xml_node<>* XMLNode);
	void processSkyDome(rapidxml::xml_node<>* XMLNode);
	void processSkyPlane(rapidxml::xml_node<>* XMLNode);
	void processClipping(rapidxml::xml_node<>* XMLNode);

	void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
	void processLightAttenuation(rapidxml::xml_node<>* XMLNode,
			Ogre::Light *pLight);

	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode,
			const Ogre::String &parameter, const Ogre::String &defaultValue =
					"");
	Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode,
			const Ogre::String &parameter, Ogre::Real defaultValue = 0);
	bool getAttribBool(rapidxml::xml_node<>* XMLNode,
			const Ogre::String &parameter, bool defaultValue = false);

	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
	Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
	Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mAttachNode;
	Ogre::String m_sGroupName;
	Ogre::String m_sPrependNode;
#ifdef USE_OGRE_TERRAIN
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::Vector3 mTerrainPosition;
#endif
	Ogre::Vector3 mLightDirection;
};
