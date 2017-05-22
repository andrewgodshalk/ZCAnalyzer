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
// #include "LeptonObject.h"
#include "Logger.h"
#include "NtupleInfo.h"
#include "SelectionProfile.h"

class EventHandler
{
  public:
    EventHandler();
    ~EventHandler(){}

    float get(const std::string,                    int i=-1) const ;
    float get(const std::string, const std::string, int i=-1) const ;
      // Intermediary functions between EH and EM to pass mapped values out of class.
      // Second function retrieves values specific for a given selection profile.

    void mapTree(TTree*);
    void addSelectionProfile(const std::string&);
    bool eventSatisfiesSelection(const std::string& selStr) const { return selectionProfiles_.at(selStr)->selectionSatisfied_; }
    void evaluateEvent();
    // bool evaluateEvent(ConfigPtr);
    // Eval Functions
    void applyGenWeight();
    void evaluateLumiJSON();

  private:
    void resetEventVariables();

    static ConfigLocator cfgLocator_;
    EventMap* evtMap_;
    NtupleInfo* currentNtupleInfo_;
    Logger logger_;

    std::map<std::string, SelectionProfile*> selectionProfiles_;   // Structs stored by specifier

};


#endif
