/*
EventMap.cpp

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

*/

// Standard Libraries
#include <iostream>
#include <string>
#include <vector>
// ROOT Classes
#include <TObjArray.h>
// Project Specific
#include "EventMap.h"

using std::map;
using std::string;
using std::vector;

EventMap::EventMap()
  : logger_("NtupleProcessor", "[EM]", 2)
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

  // Retrieve list of branches from ntuple TTree to check for existence.
    TObjArray* branchList = tree->GetListOfBranches();

  // For each branch in the config file...
    auto branches = evtMapList_->getInfoTree();
    for( auto& branch_type : branches)
    {   if(branchList->FindObject(branch_type.first.c_str()) == NULL)
        {   logger_.debug("mapTree(): BRANCH NOT FOUND IN NTUPLE: {} of type {}", branch_type.first, branch_type.second.data());
            continue;
        }
        logger_.debug("mapTree(): activating, mapping {} of type {}", branch_type.first, branch_type.second.data());
        tree->SetBranchStatus(branch_type.first.c_str(), 1);
        if(branch_type.second.data() == "Float_t"  ) tree->SetBranchAddress( branch_type.first.c_str(), &mf_[branch_type.first]);
        if(branch_type.second.data() ==   "Int_t"  ) tree->SetBranchAddress( branch_type.first.c_str(), &mi_[branch_type.first]);
        if(branch_type.second.data() == "Float_t[]") tree->SetBranchAddress( branch_type.first.c_str(), mfa_[branch_type.first]);
        if(branch_type.second.data() ==   "Int_t[]") tree->SetBranchAddress( branch_type.first.c_str(), mia_[branch_type.first]);
    }
}

float EventMap::get(string key)
{ // Look-up a map value. Return float-cast value.
    if(mf_.find(key)!=mf_.end()) return mf_.at(key);
    if(mi_.find(key)!=mi_.end()) return mi_.at(key);
    logger_.error("get(): branch \"{}\" not mapped. Returning -1000.", key);
    return -1000;
}

// Lookup for a mapped value on an array. Return float-cast value.
float EventMap::get(string key, unsigned int i)
{ // Look-up a map value. Return float-cast value.
    if(mfa_.find(key)!=mfa_.end()) return mfa_.at(key)[i];
    if(mia_.find(key)!=mia_.end()) return mia_.at(key)[i];
    logger_.error("get(): branch \"{}\" not mapped. Returning -1000.", key);
    return -1000;
}
