/*
EventMap.cpp

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

*/

// Standard Libraries
#include <iostream>
#include <string>
#include <vector>
// Project Specific
#include "EventMap.h"

using std::map;
using std::string;
using std::vector;

EventMap::EventMap() :
  logger_("NtupleProcessor", "[EM]   ")
{
    logger_.debug("EventMap Created.");
}

void EventMap::mapTree(TTree* tree)
{ // Map the input tree to variables locally
    logger_.debug("mapTree(): called");

  // Retrieve event map configuration.
    string global_evt_map_cfg_name = "current_event_map";
    evtMapList_ = cfgLocator_.getConfig(global_evt_map_cfg_name);

  // Deactivate all branches by default
    tree->SetBranchStatus("*",0);

  // For each branch in the config file...
    auto branches = evtMapList_->infoTree();
    for( auto& branch_type : branches)
    {   logger_.debug("mapTree(): activating, mapping {} of type {}", branch_type.first, branch_type.second.data());
        tree->SetBranchStatus(branch_type.first.c_str(), 1);
        if(branch_type.second.data() == "Float_t"  ) tree->SetBranchAddress( branch_type.first.c_str(), &mf_[branch_type.first]);
        if(branch_type.second.data() ==   "Int_t"  ) tree->SetBranchAddress( branch_type.first.c_str(), &mi_[branch_type.first]);
        if(branch_type.second.data() == "Float_t[]") tree->SetBranchAddress( branch_type.first.c_str(), mfa_[branch_type.first]);
        if(branch_type.second.data() ==   "Int_t[]") tree->SetBranchAddress( branch_type.first.c_str(), mia_[branch_type.first]);
    }
}
