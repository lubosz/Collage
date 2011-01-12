/*
 * AABBCollisioShape.cpp
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 *  Copyright 2010 The Collage Project
 */

#include "AABBCollisioShape.h"

AABBCollisioShape::AABBCollisioShape(Ogre::Vector2 pos, Ogre::Vector2 ext) {
	mPos = pos;
	mExt = ext;
}

AABBCollisioShape::~AABBCollisioShape() {
	// TODO(gerrit): Auto-generated destructor stub
}

void AABBCollisioShape::attachDebugVisualization(
    Ogre::SceneNode *sceneNode) {
	Ogre::ManualObject* manual = sceneNode->getCreator()->createManualObject();
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

	manual->position(mPos.x, mPos.y, 0.0);
	manual->position(mPos.x, mPos.y+mExt.y, 0.0);
	manual->position(mPos.x+mExt.x, mPos.y+mExt.y, 0.0);
	manual->position(mPos.x+mExt.x, mPos.y, 0.0);
	manual->position(mPos.x, mPos.y, 0.0);

	manual->end();
	sceneNode->attachObject(manual);
}
