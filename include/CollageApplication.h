/*
 * CollageApplication.h
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#pragma once

#include "AppStateManager.h"
#include <QtCore>
#include <QObject>
#include <QApplication>

class CollageApplication : public QApplication {
	Q_OBJECT

 public:
	CollageApplication(int& argc, char** argv); // NOLINT
	virtual ~CollageApplication();

	int exec();

 private:
	AppStateManager* m_pAppStateManager;
};

