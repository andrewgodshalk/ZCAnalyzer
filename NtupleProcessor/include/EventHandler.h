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
#include "NtupleInfo.h"

class EventHandler
{
  public:
    EventHandler();
    ~EventHandler(){}

    float get(const std::string,                    int i=-1) const ;
    float get(const std::string, const std::string, int i=-1) const ;
      // Intermediary functions between EH and EM to pass mapped values out of class.

    void mapTree(TTree*);
    void addSelectionProfile(const std::string&);
    bool eventSatisfiesSelection(const std::string& selStr) const {return eventSatisfiesSelection_.at(selStr);}
    void evaluateEvent();
    bool evaluateEvent(ConfigPtr);
    // Eval Functions
    void evaluateLumiJSON();

  private:
  // Event variables, mapped and calculated.
    EventMap* evtMap_;
    std::map<std::string, float*> calculatedVars_;
        // Variables calcualted per event. Added to event map and kept track of
        // here so that they can be reset each event.

    void resetEventVariables();

    std::map<std::string, ConfigPtr> selectionProfiles_ ;   // Configuration files containing selection information.
    std::map<std::string, bool> eventSatisfiesSelection_;   // Result of evaluation of each selection profile. Reset, checked every time process is called.

    ConfigLocator cfgLocator_     ;
    NtupleInfo* currentNtupleInfo_;
    Logger logger_;

};


#endif
