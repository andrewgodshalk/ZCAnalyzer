/*------------------------------------------------------------------------------
CutFlowTable.cpp
 Created : 2016-11-02  godshalk
 Modified: 2016-03-23  godshalk
------------------------------------------------------------------------------*/

// Project Specific classes
#include "CutFlowTable.h"

using std::pair;
using std::string;
using std::vector;
using std::to_string;

CutFlowTable::CutFlowTable()
  : HistogramExtractor(),
    logger_("NtupleProcessor", "[CF]", 2)
{   logger_.debug("CutFlowTable created.");
}

void CutFlowTable::increment(string str)
{ // Increments both integral and weighted counters for input string.
    ni_[str]++;
    nw_[str] += evt_->get("eventWeight");
}

void CutFlowTable::process()
{ // Called per event. Processes information and fills histograms.
    logger_.trace("process() called.");
    increment("EventsProcessed");
}

void CutFlowTable::terminate()
{ // Function that saves the histograms and performs any final actions before processing is completed.
    logger_.trace("terminate() called.");

  // Create histogram
    // int hSize = n_.size();
    // string hConfigStr = string("CFT_Zll_cutFlow, Cut Flow Table (Zll), Cuts, Counts, 1, 0,")+to_string(hSize);
    // TH1* htemp = rfManager_->createHistogram(hConfigStr);
    // histograms_[htemp->GetName()] = htemp;

  // Set bin name and contents for each bin.
    // int currentHBin=1;   // Visible TH1 bins start at 1. 0 is underflow.
    // for( const auto &keyAndCount : n_ )  // For every key and value in n_...
    // {   htemp->GetXaxis()->SetBinLabel(currentHBin, keyAndCount.first.c_str());
    //     htemp->SetBinContent(          currentHBin, keyAndCount.second       );
    //     currentHBin++;
    // }
  // Set some drawing options.
    // htemp->SetOption("TEXT45");

  // Perform closeout.
    printTable();
    // rfManager_->close();
}

void CutFlowTable::printTable()
{
  logger_.trace("printTable() called.");

  // Find max length of strings
    unsigned int maxKeyLength      = 0;
    unsigned int maxIntCountLength = 0;
    unsigned int maxWgtCountLength = 0;
    for(auto &kv : ni_)
    {   if(                   kv.first  .length() > maxKeyLength      ) maxKeyLength      =                    kv.first  .length();
        if(to_string(         kv.second).length() > maxIntCountLength ) maxIntCountLength = to_string(         kv.second).length();
        if(to_string((int)nw_[kv.first]).length() > maxWgtCountLength ) maxWgtCountLength = to_string((int)nw_[kv.first]).length();
    } // Second cast to int to get number of digits after decimal.
    maxWgtCountLength+=2;  // Add 2 digits for tenths decimal.

  // Print to log.
    logger_.info(" === Cut Flow Table ================");
    string countOutputFormat = "";
    countOutputFormat += "{0:>"+to_string(maxKeyLength)+"s} : {1:"+to_string(maxWgtCountLength)+".1f} ({2:"+to_string(maxIntCountLength)+"d})";
    for(auto &kv : nw_) logger_.info(countOutputFormat, kv.first, kv.second, ni_[kv.first]);

  // End output
    logger_.info(" ===================================");
    logger_.info("");
}
