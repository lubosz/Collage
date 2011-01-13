/*
 *  Copyright 2010 The Collage Project
 */
#include "LevelGenerator.h"

LevelGenerator::LevelGenerator(QObject *parent)
  :
    QObject(parent) {
    }

QString LevelGenerator::getName() {
  return this->name;
}
