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
#include "UtilFunctions.h"
// Inheriting HE Classes
#include "CutFlowTable.h"

using std::string;
using std::vector;
using utilityFunctions::getListFromString;


HistogramExtractor* HistogramExtractor::generateHistogramExtractor(string cfgStr)
{ // Function takes a string, specified in the ntupleprocessor config file,
  //   and uses it to generate a histogram extractor of the appropriate class.
    vector<string> heCfgValues;
    getListFromString(cfgStr, heCfgValues);
    // if(heCfgValues[0] == "CutFlowTable") return (HistogramExtractor*) new CutFlowTable();
    if(heCfgValues[0] == "CutFlowTable") return new CutFlowTable();
    return nullptr;
}
