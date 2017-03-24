#ifndef GUARD_CutFlowTable_h
#define GUARD_CutFlowTable_h

/*------------------------------------------------------------------------------
   CutFlowTable Class

 Created : 2016-11-02  godshalk
 Modified: 2016-03-23  godshalk

 Inherits from HistogramExtractor. Counts events passing each step of selection.
 Outputs counts to histogram.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
// ROOT Libraries
#include <TH1.h>
// Project Specific classes
#include "ConfigReader.h"
#include "EventHandler.h"
#include "HistogramExtractor.h"
#include "Logger.h"


class CutFlowTable : public HistogramExtractor
{
  public:
    CutFlowTable();
    ~CutFlowTable(){}

    void increment(std::string);
      // Increments both integral and weighted counters for input string.

    // HE Inherited/Overloaded Functions
    void process();     // Called per event. Processes information and fills histograms.
    void terminate();   // Function that saves the histograms and performs any final actions before processing is completed.


  private:
    std::map<std::string, unsigned long> ni_;   // Integer counts
    std::map<std::string,        float > nw_;   // Weighted counts (weighted by event, based on selection, sim type)

    Logger logger_;
    ConfigLocator cfgLocator_;
    ConfigPtr currentNtupleInfo_;

    void printTable();  // Prints table to log.

};

#endif
