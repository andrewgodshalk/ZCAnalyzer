#ifndef GUARD_HistogramExtractor_h
#define GUARD_HistogramExtractor_h

/*------------------------------------------------------------------------------
   HistogramExtractor Virtual Class

 Created : 2015-05-14  godshalk
 Modified: 2017-03-26  godshalk

 Virtual class used as a base for classes that extract information from ntuples
 in the form of ROOT histograms.

------------------------------------------------------------------------------*/

// Standard Libraries
// #include <map>
#include <string>
// Root Classes
// #include <TDirectory.h>
// #include <TH1.h>
// Project Specific classes
#include "EventHandler.h"
// #include "Logger.h"
#include "RootHandler.h"
// Inheriting HE Classes
#include "UtilFunctions.h"

class HistogramExtractor {
  public:
    HistogramExtractor(){}
    virtual ~HistogramExtractor(){}

    virtual void setEventHandler(EventHandler* eh)
    { // Set up HE's links to EH, including the link to its specified SP.
        evt_ = eh;
        // selectionProfile_ = evt_->getSelectionProfile(selectionProfileStr_);
    }

    virtual void process(){}     // Called per event. Processes information and fills histograms.
    virtual void terminate(){}   // Function that saves the histograms and performs any final actions before processing is completed.

    static HistogramExtractor* generateHistogramExtractor(std::string);

  protected:
    EventHandler* evt_;         // Contains event information (selection profile, mapped variables, etc.)
    RootHandler*  rh_;       // Manages output file where histograms are stored.

};

#endif
