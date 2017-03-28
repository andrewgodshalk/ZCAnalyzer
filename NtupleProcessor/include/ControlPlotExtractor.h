#ifndef GUARD_ControlPlotExtractor_h
#define GUARD_ControlPlotExtractor_h

/*------------------------------------------------------------------------------
   ControlPlotExtractor Class

 Created : 2016-11-02  godshalk
 Modified: 2017-03-27  godshalk

 Inherits from HistogramExtractor. Creates control plots of variables extracted
 from file.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <string>
#include <map>
// ROOT Libraries
#include <TH1.h>
// Project Specific classes
#include "ConfigReader.h"
#include "HistogramExtractor.h"
#include "Logger.h"

class ControlPlotExtractor : public HistogramExtractor
{
  public:
    ControlPlotExtractor(const std::string str = "ControlPlotExtractor Zll_Lp20e24");
    ~ControlPlotExtractor(){}

    // HE Inherited/Overloaded Functions
    void process();     // Called per event. Processes information and fills histograms.
    void terminate();   // Function that saves the histograms and performs any final actions before processing is completed.

  private:
    void initializeHistograms();

    Logger logger_;
    ConfigPtr cpCfg_;  // Configuration file containing plot setup information

    std::map<std::string, std::string> histFillVariables_;   // Variables from ntuples that are used to fill histograms.
};

#endif
