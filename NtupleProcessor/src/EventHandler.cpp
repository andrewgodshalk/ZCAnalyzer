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
    logger_.trace("");
    logger_.trace("evaluateEvent(): called");
    logger_.trace("evaluateEvent(): Vtype     = {}", evtMap_->get("Vtype"    ));
    logger_.trace("evaluateEvent(): nvLeptons = {}", evtMap_->get("nvLeptons"));
    logger_.trace("evaluateEvent(): naLeptons = {}", evtMap_->get("naLeptons"));

    for(int i=0; i < evtMap_->get("nvLeptons"); i++) logger_.trace("evaluateEvent(): lepton v{}: id {}, pt {}", i, evtMap_->get("vLeptons_pdgId", i), evtMap_->get("vLeptons_pt", i));
    for(int i=0; i < evtMap_->get("naLeptons"); i++) logger_.trace("evaluateEvent(): lepton a{}: id {}, pt {}", i, evtMap_->get("aLeptons_pdgId", i), evtMap_->get("aLeptons_pt", i));

  // Reset working variables.
    resetEventVariables();
}

void EventHandler::resetEventVariables()
{ // Called at beginning of evaluateEvent() to reset calculated variables to their initial, default values.
    logger_.trace("resetEventVariables(): called");
}
