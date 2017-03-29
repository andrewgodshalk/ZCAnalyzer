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
#include <list>
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
    // CutFlowTable();
    CutFlowTable(const std::string cfgStr = "CutFlowTable Zll");
    ~CutFlowTable(){}

    void increment(std::string);
      // Increments both integral and weighted counters for input string.

    // HE Inherited/Overloaded Functions
    void process();     // Called per event. Processes information and fills histograms.
    void terminate();   // Function that saves the histograms and performs any final actions before processing is completed.

  private:
    // Counts stored with key "Category.Count"
    std::map<std::string, unsigned long> ni_ ;   // Integer counts
    std::map<std::string,        float > nw_ ;   // Weighted counts (weighted by event, based on selection, sim type)
    std::list<std::pair<std::string, std::list<std::string> > > countList_;
      // Ordered list of all cut categories and the list of counts for each (i.e list<category, list<count key>)
      // Created to make the output look pretty.

    Logger logger_;
    void printTable();  // Prints table to log.

};

#endif
