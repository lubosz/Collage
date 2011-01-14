/*
 * InteractionHandler.cpp
 *
 *  Created on: 13.01.2011
 *      Author: lochmann
 */

#include "InteractionHandler.h"
#include <QtCore>

InteractionHandler::InteractionHandler() {
  // TODO Auto-generated constructor stub

}

InteractionHandler::~InteractionHandler() {
  // TODO Auto-generated destructor stub
}

void InteractionHandler::interact(Actor* a, Actor* b, float d_t){
  std::map<InteractionID, Interaction*>::iterator found;
  found = interactions.find(InteractionID(a->getTypeID(), b->getTypeID()));
  if(found == interactions.end()){
    enter(found->second);
  }else{
    inside(found->second, d_t);
  }
}

void InteractionHandler::cleanup(){
  std::map<InteractionID, Interaction*>::iterator it = interactions.begin();
  for(; it != interactions.begin(); it++){
     if(!it->second->getOk()){
       leave(it->second);
       interactions.erase(it);
     }
  }
}


void InteractionHandler::inside(Interaction* interaction, float d_t){

}

void InteractionHandler::enter(Interaction* interaction){

}

void InteractionHandler::leave(Interaction* interaction){

}
