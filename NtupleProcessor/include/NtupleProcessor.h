#ifndef GUARD_NtupleProcessor_h
#define GUARD_NtupleProcessor_h

/*------------------------------------------------------------------------------
 NtupleProcessor
   Created : 2015-09-14  godshalk
   Modified: 2017-03-20  godshalk

 Main class of NtupleProcessor program. Created to coordinate configuration and
 classes used in analyzing root ntuples.

GLOBAL CONFIGS SET BY THIS CLASS:
- current_ntuple_info - contains info on the ntuple currently being processed.
- current_event_map   - variables and types to be mapped from ttree.

------------------------------------------------------------------------------*/
// Standard Libraries
#include <exception>
#include <string>
#include <vector>
// ROOT Libraries
#include "TFile.h"
#include "TTree.h"
// Project Specific classes
#include "ConfigReader.h"
#include "HistogramExtractor.h"
#include "Logger.h"
#include "NtupleInfo.h"
#include "TimeStamp.h"
#include "TreeIterator.h"

typedef unsigned long counter;

class NtupleProcessor
{
  public:
    NtupleProcessor(int argc, char* argv[]);  // Primary constructor.
   ~NtupleProcessor(){}

  private:
    // Main work methods - breakdown of the main processes of the class.
    bool processCommandLineInput(int, char*[]);  // Processes command line options/flags.
    bool initializeConfig();
    bool initializeNtuple();
    bool initializeHistogramExtractors();
    void processNtuple();   // Sets tree iterator in motion over input ntuple.

    // Helper methods
    bool getUserYNInput(std::string query = "Would you like to continue?");
      // Function that asks the user a yes or no question.

    // Command Line Input
    std::string cfgFileName_    ;   // Configuration file name (see ZCLibrary/config/ntupleprocessor/)
    std::string ntupleLabel_    ;   // Label of ntuple to run on  (see ZCLibrary/config/ntuple/)
    bool logQuiet_              ;   // Activate/deactivate normal terminal output (warnings, errors still output)
    bool logDebug_              ;   // More detailed output in terminal output, log file.
    bool batchMode_             ;   // Whether running on batch system.
    counter eventsToProcess_    ;
    counter firstEventToProcess_;
    std::string options_        ;   // Misc. options

    // Processing information
    TFile* ntupleFile_       ;
    TTree* ntupleTree_       ;  // File and tree where ntuple is stored.
    TreeIterator* tIter_     ;
    Logger        logger_    ;
    TimeStamp     beginTime_ ;
    TimeStamp     endTime_   ;

    // Configuration info
    ConfigLocator cfgLocator_      ;
    std::string   configPath_      ;
    std::string   ntupleCfgName_   ;
    std::string   dsCfgName_       ;
    std::string   eventMapCfgName_ ;
    ConfigPtr procCfg_   ;
    ConfigPtr ntupleCfg_ ;
    ConfigPtr datasetCfg_;
    ConfigPtr eventMapInstanceCfg_;
    ConfigPtr ntupleInstanceCfg_  ;  // Config file containing
      // all information important for this instance of NtupleProcessor, including
      // information drawn from ntuple and dataset config files.

    std::vector<HistogramExtractor*> hExtractors_;
};

#endif
