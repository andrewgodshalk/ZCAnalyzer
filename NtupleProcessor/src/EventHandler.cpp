/*
EventHandler.cpp

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

*/

// Project Specific
#include "EventHandler.h"

EventHandler::EventHandler()
  : evtMap_(new EventMap()),
    logger_("NtupleProcessor", "[EH]", 1)
{   logger_.debug("EventHandler Created."); }

void EventHandler::mapTree(TTree* tree)
{ // Map the input tree to variables locally
    logger_.trace("mapTree(): called");
    evtMap_->mapTree(tree);
}

void EventHandler::evaluateEvent()
{ // Use tree map to set up physics objects
    logger_.trace("evaluateEvent(): called");
    logger_.trace("evaluateEvent(): Vtype     = {}", evtMap_->mf_["Vtype"]);
    logger_.trace("evaluateEvent(): nvLeptons = {}", evtMap_->mi_["nvLeptons"]);
    if(evtMap_->mi_["nvLeptons"] > 0)
    {
        logger_.trace("evaluateEvent(): vLeptons_pt    [0] = {}", evtMap_->mfa_["vLeptons_pt"    ][0]);
        logger_.trace("evaluateEvent(): vLeptons_charge[0] = {}", evtMap_->mia_["vLeptons_charge"][0]);
    }


  // Reset working variables.
    resetEventVariables();
}

void EventHandler::resetEventVariables()
{ // Called at beginning of evaluateEvent() to reset calculated variables to their initial, default values.
    logger_.trace("resetEventVariables(): called");
}
