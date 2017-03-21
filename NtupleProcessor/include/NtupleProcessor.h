#ifndef GUARD_NtupleProcessor_h
#define GUARD_NtupleProcessor_h

/*------------------------------------------------------------------------------
 NtupleProcessor
   Created : 2015-09-14  godshalk
   Modified: 2017-03-20  godshalk

 Main class of NtupleProcessor program. Created to coordinate configuration and
 classes used in analyzing root ntuples.

------------------------------------------------------------------------------*/
#include <exception>
#include <string>
#include <vector>
// ROOT Libraries
#include "TFile.h"
#include "TTree.h"
// Project Specific classes
// #include "ConfigLocator.h"
#include "ConfigReader.h"
#include "Logger.h"
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
    void processNtuple();   // Sets tree iterator in motion over input ntuple.

    // Helper methods
    bool getUserYNInput(std::string query = "Would you like to continue?");
      // Function that asks the user a yes or no question.

    // Command Line Input
    bool logQuiet_              ;
    bool logDebug_              ;
    counter eventsToProcess_    ;
    counter firstEventToProcess_;
    std::string options_        ;

    // File Information
    // std::string ntupleName_;
    // std::vector<std::string> ntupleFileNames_;
    // TTree*       ntuples_;
    TreeIterator* tIter_;

    // Processing information
    Logger    logger_   ;
    TimeStamp beginTime_;
    TimeStamp endTime_  ;

    // std::vector<HistogramExtractor*> hExtractors_;
};

#endif
