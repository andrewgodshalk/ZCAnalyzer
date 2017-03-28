/*------------------------------------------------------------------------------
ControlPlotExtractor.cpp
 Created : 2016-11-02  godshalk
 Modified: 2016-03-27  godshalk
------------------------------------------------------------------------------*/

// Standarad Libraries
#include <string>
// ROOT Classes
#include <TFile.h>
#include <TH1F.h>
// Project Specific classes
#include "ControlPlotExtractor.h"
#include "UtilFunctions.h"

using std::string;

ControlPlotExtractor::ControlPlotExtractor(const string cfgStr)
  : HistogramExtractor(cfgStr),
    logger_("NtupleProcessor", "[CPE]", 2)
{   logger_.debug("ControlPlotExtractor created.");
}

void ControlPlotExtractor::process()
{ // Called per event. Processes information and fills histograms.
    logger_.trace("process() called.");
}

void ControlPlotExtractor::terminate()
{ // Function that saves the histograms and performs any final actions before processing is completed.
    logger_.trace("terminate() called.");

  // Close root handler.
    rh_->close();
}
