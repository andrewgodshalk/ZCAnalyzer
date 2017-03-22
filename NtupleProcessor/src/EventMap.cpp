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
    logger_.trace("EventMap Created.");
}

void EventMap::mapTree(TTree* tree)
{ // Map the input tree to variables locally
    logger_.trace("mapTree(): called");

  // List all active branches.
    tree->SetBranchStatus("*",0);  // Deactivates all branches.
    vector<string> eventBranches =
    {    "Vtype"    };
    logger_.trace("mapTree(): branch lists created.");

  // Deactivate all branches, reactivate as necessary.
    for(const string& br : eventBranches) tree->SetBranchStatus(br.c_str(), 1);
    logger_.trace("mapTree(): branch lists reactivated.");

  // Map tree
    tree->SetBranchAddress( "Vtype", &Vtype_ );
    logger_.trace("mapTree(): branch variables mapped.");
}
