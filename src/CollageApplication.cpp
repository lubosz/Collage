/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "CollageApplication.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"

CollageApplication::CollageApplication() {
}

CollageApplication::~CollageApplication() {
}

void CollageApplication::start() {
	RenderEngine::Instance().initOgre("Collage");
	Input::Instance().initOis();
    UserInterface::Instance().initOgreBites(
    		RenderEngine::Instance().
    		m_pRenderWnd,
    		Input::Instance().m_pMouse
	);
}

int main(int argc, char *argv[]) {
	CollageApplication collage;
	try {
		collage.start();
	}
	catch(const std::exception& e) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
}
