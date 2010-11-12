/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *      Author: bmonkey
 */

#include "CollageApplication.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"

CollageApplication::CollageApplication() {
	// TODO Auto-generated constructor stub

}

CollageApplication::~CollageApplication() {
	// TODO Auto-generated destructor stub
}

void CollageApplication::start(){
	RenderEngine::Instance().initOgre("Collage");
	Input::Instance().initOis();
    UserInterface::Instance().initOgreBites(RenderEngine::Instance().m_pRenderWnd, Input::Instance().m_pMouse);
}
