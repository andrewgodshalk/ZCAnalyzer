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
    configStr_(cfgStr)
{
  // Split config string
    getListFromString(configStr_, cfgValues_);

  // set up RootHandler
    string rhFileName = "output/";
    if(cfgValues_[0] == "CutFlowTable"        ) rhFileName += "CFT_";
    if(cfgValues_[0] == "ControlPlotExtractor") rhFileName += "CPE_";
    rhFileName += currentNtupleInfo_->label + "_" + cfgValues_[1] + ".root";
    rh_ = new RootHandler(rhFileName);
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
