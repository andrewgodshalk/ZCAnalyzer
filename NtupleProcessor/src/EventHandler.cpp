/*
EventHandler.cpp

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

*/

// Project Specific
#include "EventHandler.h"

using std::map;
using std::string;

// Initialize statics
ConfigLocator EventHandler::cfgLocator_;

EventHandler::EventHandler()
  : evtMap_(new EventMap()),
    // currentNtupleInfo_(cfgLocator_.getConfig("current_ntuple_info")),
    currentNtupleInfo_(NtupleInfo::getInstance()),
    logger_("NtupleProcessor", "[EH]", 1)
{   logger_.debug("EventHandler Created.");

}

float EventHandler::get(const string key, int i) const
{ // Intermediary function between EH and EM to pass mapped values out of class.
    if(i<0)
        return evtMap_->get(key   );
    return     evtMap_->get(key, i);
}

float EventHandler::get(const string selectionProfile, const string key, int i) const
{ // Retrieves calculated values from this event using the value key and the selection profile
    if(i<0) return selectionProfiles_.at(selectionProfile)->calculatedValues_.at(key)   ;
    else    return selectionProfiles_.at(selectionProfile)->calculatedArrays_.at(key)[i];
}

void EventHandler::mapTree(TTree* tree)
{ // Map the input tree to variables locally
    logger_.trace("mapTree(): called");
    evtMap_->mapTree(tree);

  // Set up calculated variables.
    for( auto& sp : selectionProfiles_ )
    {
      sp.second->calculatedValues_["genSign"     ] = 0;
      sp.second->calculatedValues_["eventWeight" ] = 0;
      sp.second->calculatedValues_["jsonFromFile"] = 0;
    }

}

void EventHandler::addSelectionProfile(const std::string& selStr)
{ // Allows histogram extractors can add their particular selection to the list of those to be checked.
    logger_.trace("addSelectionProfile(): add {}", selStr);
    selectionProfiles_[selStr] = new SelectionProfile(selStr);
}

void EventHandler::evaluateEvent()
{ // Use tree map to set up physics objects
    logger_.trace("");
    logger_.trace("evaluateEvent(): called");
  // Reset working variables.
    resetEventVariables();

  // Set up event weight based on sign from generation, or just set = 1.0.
  // if(currentNtupleInfo_->get<string>("data_or_sim") == "sim")
    if(currentNtupleInfo_->isSim)
        for( auto& sp : selectionProfiles_ )
        {     sp.second->calculatedValues_["genSign"    ]
            = sp.second->calculatedValues_["eventWeight"]
            = (get("genWeight") < 0 ? -1 : 1);
        }
    else for( auto& sp : selectionProfiles_ ) sp.second->calculatedValues_["eventWeight"] = 1.0;

}

void EventHandler::evaluateLumiJSON()
{ // Check if event falls within "good" luminosity sections.

}

void EventHandler::resetEventVariables()
{ // Called at beginning of evaluateEvent() to reset calculated variables to their initial, default values.
    logger_.trace("resetEventVariables(): called");
    for( auto& sp : selectionProfiles_ )
    {
        for(auto& kv : sp.second->calculatedValues_ ) kv.second = 0;
        for(auto& kv : sp.second->calculatedArrays_ ) kv.second.clear();
        sp.second->selectionSatisfied_ = false;
    }
}
