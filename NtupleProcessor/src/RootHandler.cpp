/*------------------------------------------------------------------------------
RootHandler.cpp
 Created : 2016-11-03  godshalk
 Modified: 2017-03-24  godshalk
------------------------------------------------------------------------------*/

// Standard Libraries
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iterator>
#include <vector>
// Project Specific classes
#include "RootHandler.h"

using std::string;
using std::find;
using std::find_if;
using std::reverse_iterator;
using std::vector;

RootHandler::RootHandler(string f)
  : fileName_(f), // filePath_("output/"),
    // hProps_(10,""),
    logger_("NtupleProcessor", "[RF]", 3)
{ //
    logger_.debug("RootHandler created.");
    logger_.debug("File name: {}", fileName_);
    // logger_.debug("File path: {}", filePath_);

  // Open root file.
    logger_.debug("Opening file {}", fileName_.c_str() );
    file_ = TFile::Open(fileName_.c_str(), "RECREATE");
    if(!file_->IsOpen()) logger_.critical("Unable to open file {}", fileName_);
}

void RootHandler::addHistogram(const string str, TH1* h)
{ // Creates a clone of the given histogram with the given name.
    file_->cd();   histograms_[str] = (TH1*) h->Clone(str.c_str());
}


TH1* RootHandler::createHistogram(const string key, const vector<string>& hProps)
{ // Creates a histogram within the root file, based on a configuration given.
  // hProps contains:
    logger_.trace("createHistogram() called with key {}", key);
    TH1* htemp = NULL;

    string hName  = hProps[0];
    string hTitle = hProps[1] + ";" + hProps[2] + ";" + hProps[3];
    float hBinSize  = strtof(hProps[4].c_str(), NULL);
    float hMinRange = strtof(hProps[5].c_str(), NULL);
    float hMaxRange = strtof(hProps[6].c_str(), NULL);
    int nBins = (hMaxRange-hMinRange)/hBinSize;
  // If bin size isn't exact, make max range exact.
    float calcedMax = hMinRange+(hBinSize*nBins);
    if( hMaxRange != calcedMax ) hMaxRange = calcedMax;

  // Move to file within ROOT framework.
    file_->cd();

  // Create a histogram based on preferences.
    if(hProps.back() == "int")
         htemp = new TH1I(hName.c_str(), hTitle.c_str(), nBins, hMinRange, hMaxRange);
    else htemp = new TH1F(hName.c_str(), hTitle.c_str(), nBins, hMinRange, hMaxRange);

  // // Store histogram locally and return a pointer.
    histograms_[key] = htemp;
    return htemp;
}

void RootHandler::close()
{ // Saves and closes the root file.
    logger_.trace("RootHandler closing file: {}", fileName_);
    file_->cd();
    for( auto &nameAndHisto : histograms_ ) nameAndHisto.second->Write();
    file_->Close();
}
