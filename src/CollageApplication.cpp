/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "CollageApplication.h"
#include "Input.h"

CollageApplication::CollageApplication() {
	// TODO Auto-generated constructor stub

}

CollageApplication::~CollageApplication() {
	// TODO Auto-generated destructor stub
}

void CollageApplication::start(){
	RenderEngine::getSingleton().initOgre("Collage");
	Input::getSingleton().initOis();
    UserInterface::getSingleton().initOgreBites(m_pRenderWnd, m_pMouse);
}
