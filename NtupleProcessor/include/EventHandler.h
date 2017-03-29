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
    bool eventSatisfiesSelection(const std::string& selStr) const { return selectionProfiles_.at(selStr)->selectionSatisfied_; }
    void evaluateEvent();
    bool evaluateEvent(ConfigPtr);
    // Eval Functions
    void evaluateLumiJSON();

  private:
    void resetEventVariables();

    static ConfigLocator cfgLocator_;
    EventMap* evtMap_;
    NtupleInfo* currentNtupleInfo_;
    Logger logger_;

    struct SelectionProfile
    {
        std::string specifier_;   // String that specifies what selction profile to use.
        std::map<std::string,             float  > calculatedValues_;
        std::map<std::string, std::vector<float> > calculatedArrays_;
          // Values calculated on the fly. Second list contains arrays of variables. One set of variables for each selection profile stored.
        ConfigPtr cfg_;            // Configuration files containing selection information.
        bool selectionSatisfied_;  // Result of evaluation of each selection profile. Reset, checked every time process is called.
        SelectionProfile(const std::string& spec)
          : specifier_(spec),
            cfg_(cfgLocator_.getConfig(std::string("ZCLibrary/config/selection/")+spec+".ini"))
        {}
    };
    std::map<std::string, SelectionProfile*> selectionProfiles_;   // Structs stored by specifier

};


#endif
