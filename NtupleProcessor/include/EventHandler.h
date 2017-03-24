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
#include <map>
#include <string>

// Root Classes
#include <TTree.h>
// Project Specific classes
#include "ConfigReader.h"
#include "EventMap.h"
#include "Logger.h"

class EventHandler
{
  public:
    EventHandler();
    ~EventHandler(){}

    float get(std::string, int i=-1);
      // Intermediary function between EH and EM to pass mapped values out of class.

    void mapTree(TTree*);
    void evaluateEvent();

  private:
  // Event variables, mapped and calculated.
    EventMap* evtMap_;
    std::map<std::string, float*> calculatedVars_;
        // Variables calcualted per event. Added to event map and kept track of
        // here so that they can be reset each event.

    void resetEventVariables();

    ConfigLocator cfgLocator_;
    ConfigPtr currentNtupleInfo_;
    Logger logger_;

};


#endif
