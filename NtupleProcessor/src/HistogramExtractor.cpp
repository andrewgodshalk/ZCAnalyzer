/*------------------------------------------------------------------------------
HistogramExtractor.cpp
 Created : 2017-03-26  godshalk
 Modified: 2017-03-26  godshalk
------------------------------------------------------------------------------*/

// Standard Libraries
#include <string>
#include <vector>
// Project Specific classes
#include "HistogramExtractor.h"
#include "NtupleInfo.h"
#include "UtilFunctions.h"
// Inheriting HE Classes
#include "ControlPlotExtractor.h"
#include "CutFlowTable.h"

using std::string;
using std::vector;
using utilityFunctions::getListFromString;


HistogramExtractor::HistogramExtractor(const std::string& cfgStr)
  : currentNtupleInfo_(NtupleInfo::getInstance()),
    cfgStr_(cfgStr)
{
  // Get config variables from string, initialize members
    getListFromString(cfgStr_, cfgValues_);
    classStr_ = cfgValues_[0];
    selStr_   = cfgValues_[1];
    if(classStr_ == "CutFlowTable"        ) abbrevStr_ = "CFT_";
    if(classStr_ == "ControlPlotExtractor") abbrevStr_ = "CPE_";

  // set up RootHandler
    rhFileName_ = string("output/") + abbrevStr_ + currentNtupleInfo_->label + "_" + selStr_ + ".root";
    rh_ = new RootHandler(rhFileName_);
}

HistogramExtractor* HistogramExtractor::generateHistogramExtractor(string cfgStr)
{ // Function takes a string, specified in the ntupleprocessor config file,
  //   and uses it to generate a histogram extractor of the appropriate class.
    vector<string> heCfgValues;
    getListFromString(cfgStr, heCfgValues);
    if(heCfgValues[0] == "CutFlowTable"        ) return new CutFlowTable(        cfgStr);
    if(heCfgValues[0] == "ControlPlotExtractor") return new ControlPlotExtractor(cfgStr);
    return nullptr;
}
