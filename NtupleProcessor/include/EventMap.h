#ifndef GUARD_EventMap_h
#define GUARD_EventMap_h

/*------------------------------------------------------------------------------
   EventMap

 Created : 2016-10-25  godshalk
 Modified: 2017-03-21  godshalk

 Maps event values in a TTree into structs/classes/variables,
 which are then accessed by EventHandler.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <map>
#include <string>
#include <vector>
// Root Classes
#include <TChain.h>
// Project Specific classes
#include "Logger.h"

struct EventMap
{
  public:
    EventMap();
    ~EventMap(){}

    void mapTree(TTree*);

    // Map variables
    Float_t Vtype_;

  private:
    Logger logger_;

};

#endif
