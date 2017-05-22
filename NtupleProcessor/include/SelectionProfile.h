#ifndef GUARD_SelectionProfile_h
#define GUARD_SelectionProfile_h

/*------------------------------------------------------------------------------
   EventHandler

   Modified: 2017-04-28  godshalk
   Created : 2017-04-28  godshalk

------------------------------------------------------------------------------*/


// Standard Libraries
#include <map>
#include <string>
#include <vector>
// Project Specific classes
#include "ConfigReader.h"
#include "Logger.h"
#include "NtupleInfo.h"
#include "LeptonObject.h"


class SelectionProfile
{
  public:
    SelectionProfile(const std::string& spec)
      : specifier_(spec),
        cfg_(cfgLocator_.getConfig(std::string("ZCLibrary/config/selection/")+spec+".ini")),
        logger_("NtupleProcessor", "[SP]", 2)
    {}

    std::string specifier_;   // String that specifies what selction profile to use.
    std::map<std::string,             float  > calculatedValues_;
    std::map<std::string, std::vector<float> > calculatedArrays_;
    std::map<std::string, std::vector<LeptonObject*> > lepLists_;
      // Values calculated on the fly. Second list contains arrays of variables. One set of variables for each selection profile stored.
      // Multiple lep lists for different leptons. For instance, all leptons, leptons from z-boson, leptons not from z-boson, muons, electrons, etc.
    ConfigPtr cfg_;            // Configuration files containing selection information.
    bool selectionSatisfied_;  // Result of evaluation of each selection profile. Reset, checked every time process is called.

  private:
    ConfigLocator cfgLocator_;

  // Evaluater Classes, set up from config file values.
    // LumiJSONReader
    Logger logger_;
};

#endif
