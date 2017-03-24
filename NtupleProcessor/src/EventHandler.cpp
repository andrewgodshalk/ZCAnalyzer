/*
EventHandler.cpp

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

*/

// Project Specific
#include "EventHandler.h"

using std::map;
using std::string;

EventHandler::EventHandler()
  : evtMap_(new EventMap()),
    logger_("NtupleProcessor", "[EH]", 1)
{   logger_.debug("EventHandler Created."); }

float EventHandler::get(string key, int i)
{ // Intermediary function between EH and EM to pass mapped values out of class.
    if(i<0)
        return evtMap_->get(key   );
    return     evtMap_->get(key, i);
}

void EventHandler::mapTree(TTree* tree)
{ // Map the input tree to variables locally
    logger_.trace("mapTree(): called");
    evtMap_->mapTree(tree);

  // Set up pointers for calculated variables.
    calculatedVars_["genSign"    ] = & evtMap_->mf_["genSign"    ];
    calculatedVars_["eventWeight"] = & evtMap_->mf_["eventWeight"];
}

void EventHandler::evaluateEvent()
{ // Use tree map to set up physics objects
    logger_.trace("");
    logger_.trace("evaluateEvent(): called");
  // Reset working variables.
    resetEventVariables();

  // Some debug output
    // logger_.trace("evaluateEvent(): Vtype     = {}", evtMap_->get("Vtype"    ));
    // logger_.trace("evaluateEvent(): nvLeptons = {}", evtMap_->get("nvLeptons"));
    // logger_.trace("evaluateEvent(): naLeptons = {}", evtMap_->get("naLeptons"));
    // for(int i=0; i < evtMap_->get("nvLeptons"); i++) logger_.trace("evaluateEvent(): lepton v{}: id {}, pt {}", i, evtMap_->get("vLeptons_pdgId", i), evtMap_->get("vLeptons_pt", i));
    // for(int i=0; i < evtMap_->get("naLeptons"); i++) logger_.trace("evaluateEvent(): lepton a{}: id {}, pt {}", i, evtMap_->get("aLeptons_pdgId", i), evtMap_->get("aLeptons_pt", i));

  // Calculate some extra variables.
    *(calculatedVars_["genSign"    ]) = (get("genWeight") < 0 ? -1 : 1);
    *(calculatedVars_["eventWeight"]) = *(calculatedVars_["genSign"]);

}

void EventHandler::resetEventVariables()
{ // Called at beginning of evaluateEvent() to reset calculated variables to their initial, default values.
    logger_.trace("resetEventVariables(): called");
    for(auto& kv : calculatedVars_)  *(kv.second) = 0;
}
