/*
 * Simulation.h
 *
 *  Created on: 22.12.2010
 *      Author: lochmann
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "AABB.h"

typedef std::vector<AABB> AABBList;

class Simulation {
public:
	Simulation();
	virtual ~Simulation();

	AABBList *mAABBList;
};

#endif /* SIMULATION_H_ */
