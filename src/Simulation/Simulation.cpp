/*
 * Simulation.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#include "Simulation.h"
#include <QtCore>
#include "Actor.h"

Simulation::Simulation(Ogre::SceneNode *rootSceneNode) {
	// TODO Auto-generated constructor stub
	mRootSceneNode = rootSceneNode;

	Ogre::MeshManager::getSingleton().createPlane(
			"DebugVisualization",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::Plane(0, 0, 1, 0),
			1,1
	);
}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
}

Actor* Simulation::createActor(const Ogre::String name, ActorBehavior behavior, Ogre::Vector2 position){
	Ogre::SceneNode *sceneNode = mRootSceneNode->createChildSceneNode(name, Ogre::Vector3(position.x, position.y, 0.0));
	Actor *actor = new Actor(behavior, sceneNode);
	if(behavior == AB_STATIC){
		mActorListStatic.push_back(actor);
	}else{
		mActorListDynamic.push_back(actor);
	}
	if(mDebugVisualization){
		Ogre::Entity *debugEntity = sceneNode->getCreator()->createEntity("debug_"+name, "DebugVisualization");
		sceneNode->attachObject(debugEntity);
	}
	return actor;
}

void Simulation::update(float d_t){
	foreach(Actor *dyn, mActorListDynamic){
		dyn->update(d_t);
	}

//	ActorList::iterator dyn1 = mActorListDynamic.begin();
//	for(;it < mActorListDynamic.end(); it++){
//		ActorList::iterator dyn2 = it+1;
//		for(;it2 < mActorDynamic.end(); it++){
//
//		}
//	}

	foreach(Actor *dyn, mActorListDynamic){
		foreach(Actor *sta, mActorListStatic){

		}
	}
}

Ogre::Vector2 Simulation::getGravity() const
{
    return mGravity;
}

void Simulation::setGravity(Ogre::Vector2 mGravity)
{
    this->mGravity = mGravity;
}


