/*------------------------------------------------------------------------------
CutFlowTable.cpp
 Created : 2016-11-02  godshalk
 Modified: 2016-03-23  godshalk
------------------------------------------------------------------------------*/

// ROOT Classes
#include <TFile.h>
#include <TH1F.h>
// Project Specific classes
#include "CutFlowTable.h"
#include "UtilFunctions.h"

using std::pair;
using std::string;
using std::vector;
using std::to_string;

CutFlowTable::CutFlowTable()
  : HistogramExtractor(""),
    logger_("NtupleProcessor", "[CF]", 2)
{   logger_.debug("CutFlowTable created.");
}

CutFlowTable::CutFlowTable(const string& cfgStr)
  : HistogramExtractor(cfgStr),
    logger_("NtupleProcessor", "[CF]", 2)
{   logger_.debug("CutFlowTable created from string: {}", cfgStr);
  // Create histogram from input string.

}

void CutFlowTable::increment(string str)
{ // Increments both integral and weighted counters for input string.
    ni_[str]++;
    nw_[str] += evt_->get("eventWeight");
}

void CutFlowTable::process()
{ // Called per event. Processes information and fills histograms.
    logger_.trace("process() called.");
    increment("Events Processed");
}

void CutFlowTable::terminate()
{ // Function that saves the histograms and performs any final actions before processing is completed.
    logger_.trace("terminate() called.");

  // Add numbers from ntuple config to the counts recorded.
    ni_["Tree Entries"] = nw_["Tree Entries"] = currentNtupleInfo_->ntupleEvents;
    nw_["Original Events Processed"] = currentNtupleInfo_->netDSEvents;
    ni_["Original Events Processed"] = currentNtupleInfo_->absDSEvents;
    // If there are no events in net_events (i.e. not a sim ntuple(?)) set equal to abs_counts.
    if(nw_["Original Events Processed"] == 0) nw_["Original Events Processed"] = ni_["Original Events Processed"];

  // Create histogram (TEST)
    // TFile* f_output = TFile::Open("output/cft_test.root", "RECREATE");
    // f_output->cd();
    int hSize = ni_.size();
    TH1F* htemp_int = new TH1F("CFT_int_counts", "Cut Flow Table (Int);Cuts;Events", hSize, 0, hSize);
    TH1F* htemp_wgt = new TH1F("CFT_wgt_counts", "Cut Flow Table (Wgt);Cuts;Events", hSize, 0, hSize);

  // Set bin name and contents for each bin.
    int currentHBin=1;   // Visible TH1 bins start at 1. 0 is underflow.
    for( const auto &keyAndCount : ni_ )  // For every key and value in n_...
    {   htemp_int->GetXaxis()->SetBinLabel(currentHBin, keyAndCount.first.c_str());
        htemp_int->SetBinContent(          currentHBin, keyAndCount.second       );
        currentHBin++;
    }
    currentHBin=1;   // Visible TH1 bins start at 1. 0 is underflow.
    for( const auto &keyAndCount : nw_ )  // For every key and value in n_...
    {   htemp_wgt->GetXaxis()->SetBinLabel(currentHBin, keyAndCount.first.c_str());
        htemp_wgt->SetBinContent(          currentHBin, keyAndCount.second       );
        currentHBin++;
    }

  // Set some drawing options.
    htemp_int->SetOption("B TEXT45");
    htemp_wgt->SetOption("B TEXT45");

  // Add plots to root handler and close.
    rh_->addHistogram(htemp_int, "integral_counts");
    rh_->addHistogram(htemp_wgt, "weighted_counts");
    rh_->close();

  // Perform closeout.
    printTable();
    delete htemp_int;
    delete htemp_wgt;

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
