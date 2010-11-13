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

int main(int argc, char *argv[])
{
	CollageApplication collage;
	try
	{
		collage.start();
	}
	catch(std::exception& e)
    {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }

}
