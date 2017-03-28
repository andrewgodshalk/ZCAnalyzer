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

using std::map;
using std::string;
using std::vector;
using utilityFunctions::getDelimitedListFromString;

ControlPlotExtractor::ControlPlotExtractor(const string cfgStr)
  : HistogramExtractor(cfgStr),
    logger_("NtupleProcessor", "[CPE]", 2)
{   logger_.debug("ControlPlotExtractor created.");

  // Get config file. Custom config files can be specified in NP configuration after selection.
    string cfgFilename = "ZCLibrary/config/controlplots/";
    if(cfgValues_.size() >= 3) cfgFilename += cfgValues_[2];
    else cfgFilename+= "default.ini";
    logger_.debug("Opening config file {}", cfgFilename);
    cpCfg_ = ConfigLocator().getConfig(cfgFilename);
    initializeHistograms();
}

void ControlPlotExtractor::initializeHistograms()
{ // Initialize control plots from configuration file listings.
    logger_.debug("CPE Cfg: Reading lines:");
    auto cp_strings = cpCfg_->getInfoTree();
    vector<string> histProps;
    for( const auto& cp_str : cp_strings)
    {   logger_.debug("  {} = {}", cp_str.first, cp_str.second.data());
        getDelimitedListFromString(cp_str.second.data(), histProps);
        logger_.debug("  {} = {} | {} | {} | {} | {} | {} | {} | {}", cp_str.first, histProps[0], histProps[1], histProps[2], histProps[3], histProps[4], histProps[5], histProps[6], histProps[7] );
        histFillVariables_[cp_str.first] = histProps.back();
        rh_->createHistogram(cp_str.first, histProps);
    }
}

void ControlPlotExtractor::process()
{ // Called per event. Processes information and fills histograms.
    logger_.trace("process() called.");
    for( auto& hist_fill : histFillVariables_)    // Cycle through histogram fill instructions.
        if(hist_fill.second != "")  // If a variable is given, fill with the event weight.
            rh_->getHist(hist_fill.first)->Fill(evt_->get(hist_fill.second), evt_->get("eventWeight"));
}

void ControlPlotExtractor::terminate()
{ // Function that saves the histograms and performs any final actions before processing is completed.
    logger_.trace("terminate() called.");

  // Close root handler.
    rh_->close();
}
