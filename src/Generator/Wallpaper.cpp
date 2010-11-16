/*
 * Wallpaper.cpp
 *
 *  Created on: Nov 16, 2010
 *      Author: bmonkey
 */

#include "Wallpaper.h"
#include "FrameCapture.h"
#include <OGRE/Ogre.h>
#include <QCoreApplication>

Wallpaper::Wallpaper() {
    QUrl url = QUrl("http://wallbase.net/random");
    QString fileName = "wallbase.png";
    char * argv;
    int argc = 0;
    QCoreApplication a(argc,&argv);
    FrameCapture capture;
    capture.load(url, fileName);

    a.exec();
}

Wallpaper::~Wallpaper() {
	// TODO Auto-generated destructor stub
}
