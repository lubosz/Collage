/*
 *  Copyright 2010 The Collage Project
 */
#include "Level.h"

Level::Level(QObject *parent)
:
    QObject(parent) {
  this->simulation = NULL;
}

Level::Level(Simulation *simulation) {
    this->simulation = simulation;
}
