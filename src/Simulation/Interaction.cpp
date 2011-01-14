/*
 *  Copyright 2010 The Collage Project
 */
#include "Interaction.h"

Interaction::Interaction(Actor* a, Actor* b) {
  this->a = a;
  this->b = b;
}

Interaction::~Interaction() {
}

Actor *Interaction::getA() const {
    return a;
}

Actor *Interaction::getB() const {
    return b;
}

bool Interaction::getOk() const {
    return ok;
}

void Interaction::setOk(bool ok) {
    this->ok = ok;
}


