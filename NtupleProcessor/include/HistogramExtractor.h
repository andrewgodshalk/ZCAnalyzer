#ifndef GUARD_HistogramExtractor_h
#define GUARD_HistogramExtractor_h

/*------------------------------------------------------------------------------
   HistogramExtractor Virtual Class

 Created : 2015-05-14  godshalk
 Modified: 2017-03-23  godshalk

 Virtual class used as a base for classes that extract information from ntuples
 in the form of ROOT histograms.

------------------------------------------------------------------------------*/

// Standard Libraries
// #include <map>
// #include <string>
// Root Classes
// #include <TDirectory.h>
// #include <TH1.h>
// Project Specific classes
#include "EventHandler.h"
// #include "Logger.h"
// #include "RootFileManager.h"


class HistogramExtractor {
  public:
    // HistogramExtractor(std::string n, std::string c, std::string s = "")
    //  : ntupleName_(n), className_(c), selectionProfileStr_(s)
    HistogramExtractor()
    { // Set up RootFileManager
        // std::string fileName = className_ + "_" + ntupleName_ + "_" + selectionProfileStr_ + ".root";
        // rfManager_ = new RootFileManager(fileName);
    }
    virtual ~HistogramExtractor(){}

    virtual void setEventHandler(EventHandler* eh)
    { // Set up HE's links to EH, including the link to its specified SP.
        evt_ = eh;
        // selectionProfile_ = evt_->getSelectionProfile(selectionProfileStr_);
    }

    virtual void process(){}     // Called per event. Processes information and fills histograms.
    virtual void terminate(){}   // Function that saves the histograms and performs any final actions before processing is completed.

  protected:
  //   std::string ntupleName_;
  //   std::string className_;            // Name of this histogram extractor class.
  //   std::string selectionProfileStr_;  // Base selection profile used for histogram collection.
    EventHandler* evt_;                // Contains event information (selection profile, mapped variables, etc.)
  //   RootFileManager* rfManager_;       // Manages output file where histograms are stored.
  //   SelectionProfile* selectionProfile_;
  //
  // Histogram collection
    // std::map<std::string, TH1*> histograms_;   // List of histograms that will be filled by the fillHistos function

};

#endif
