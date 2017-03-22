#ifndef GUARD_EventHandler_h
#define GUARD_EventHandler_h

/*------------------------------------------------------------------------------
   EventHandler

   Modified: 2017-03-21  godshalk
 Created : 2016-10-25  godshalk

 Intermediary between the mapped tree entry and the tree processor and extractor.
 Does intermediate calculations, including calculating important values,
 performing selection, and sorting/organizing physics object arrays.
------------------------------------------------------------------------------*/

// Standard Libraries
// Root Classes
#include <TTree.h>
// Project Specific classes
#include "EventMap.h"
#include "Logger.h"

class EventHandler
{
  public:
    EventHandler();
    ~EventHandler(){}

    void mapTree(TTree*);

    void evaluateEvent();
    EventMap* evtMap_;

  private:
    Logger logger_;

    void resetEventVariables();

};


#endif
