/*
 * Interaction.cpp
 *
 *  Created on: 14.01.2011
 *      Author: lochmann
 */

#include "Interaction.h"

Interaction::Interaction(Actor* a, Actor* b) {
  // TODO Auto-generated constructor stub
  this->a = a;
  this->b = b;
}

Interaction::~Interaction() {
  // TODO Auto-generated destructor stub
}

Actor *Interaction::getA() const
{
    return a;
}

Actor *Interaction::getB() const
{
    return b;
}

bool Interaction::getOk() const
{
    return ok;
}

void Interaction::setOk(bool ok)
{
    this->ok = ok;
}


