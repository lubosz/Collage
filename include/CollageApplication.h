/*
 * CollageApplication.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppStateManager.h"

class CollageApplication {
 public:
	CollageApplication();
	virtual ~CollageApplication();

	void start();

 private:
	AppStateManager* m_pAppStateManager;
};

