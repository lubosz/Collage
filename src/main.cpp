/*
 * main.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#include "CollageApplication.h"

int main(int argc, char *argv[]) {
	CollageApplication collage(argc, argv);

		try {
			collage.exec();
		}
		catch(const std::exception& e) {
	        fprintf(stderr, "An exception has occurred: %s\n", e.what());
	    }
}
