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

TH1* RootHandler::createHistogram(string configStr)
{ // Creates a histogram within the root file, based on a configuration string.
    logger_.trace("createHistogram() called with configStr {}", configStr);
  //   if(!getHistoPropsFromString(configStr)) return NULL;
  //
  //   string hName  = hProps_[0];
  //   string hTitle = (hProps_[1]+";"+hProps_[2]+";"+hProps_[3]).c_str();
  //   float hBinSize  = strtof(hProps_[4].c_str(), NULL);
  //   float hMinRange = strtof(hProps_[5].c_str(), NULL);
  //   float hMaxRange = strtof(hProps_[6].c_str(), NULL);
  //   int nBins = (hMaxRange-hMinRange)/hBinSize;
  // // If bin size isn't exact, make max range exact.
  //   float calcedMax = hMinRange+(hBinSize*nBins);
  //   if( hMaxRange != calcedMax ) hMaxRange = calcedMax;

  // Move to file within ROOT framework.
    file_->cd();

  // Create a histogram based on preferences.
    TH1* htemp = NULL;
  //   if(hProps_.back() == "int")
  //        htemp = new TH1I(hName.c_str(), hTitle.c_str(), nBins, hMinRange, hMaxRange);
  //   else htemp = new TH1F(hName.c_str(), hTitle.c_str(), nBins, hMinRange, hMaxRange);
  //
  // // Store histogram locally and return a pointer.
  //   histograms_[hName] = htemp;
    return htemp;
}

void RootHandler::close()
{ // Saves and closes the root file.
    logger_.trace("RootHandler closing file: {}", fileName_);
    file_->cd();
    // for( auto &nameAndHisto : histograms_ ) nameAndHisto.second->Write();
    file_->Close();
}

bool RootHandler::getHistoPropsFromString(const string& cfgStr)
{ // Pulls histogram properties from the input string, stores in properties vector.;
  // Configuration string formats:
  // <name> ,<title> ,<xlabel> ,<ylabel> ,<binsize> ,<min> ,<max> [,<type>] [,other options][,]
  // - type float by default. Can also say int.
  // - properties are delimited by semicolons.
  // - extra space before or after the semicolon will be ignored.
    logger_.trace("getHistoPropsFromString() called with config {}", cfgStr);

  // // Initialize working variables, storage.
  //   typedef string::const_iterator str_citer;
  //   typedef reverse_iterator<string::const_iterator> str_rciter;
  //   str_citer propBegin, propEnd, substrBegin, substrEnd;
  //   propBegin = propEnd = substrBegin = substrEnd = cfgStr.begin();
  //   str_citer strEnd = cfgStr.end();
  //   hProps_.clear(); // Properties extracted.
  //
  //   while(propBegin!=strEnd) // Until the end of the string has been met...
  //   { // Locate the boundaries of the next property substring.
  //       propEnd     = find(       propBegin,  strEnd,     ','); // Find the end of the current property listing.
  //       substrBegin = find_if_not(propBegin, propEnd, isspace); // Find the first non-whitespace character to begin the propery.
  //       substrEnd   = find_if_not( str_rciter(propEnd), str_rciter(substrBegin), isspace).base();
  //         // Find the last non-whitespace character, the end of the property substring
  //       hProps_.push_back(string(substrBegin, substrEnd));
  //     // Break out if at the end of the string.
  //       if(propEnd == strEnd) break;
  //     // Otherwise, reset iterators to the next character after the end of the current property.
  //       propBegin = substrBegin = substrEnd = propEnd+1;
  //   }
  //
  // // Test property extraction
  //   string testStr = "";
  //   for(string& s: hProps_) testStr += s + "|";
  //   logger_.debug("getHistoPropsFromString(): separated properties: {}", testStr);
  //
  // // Check to make sure the correct number of properties are stored. Return false if not.
  //    if(hProps_.size() < 7 || 9 < hProps_.size())
  //    {   logger_.error("getHistoPropsFromString(): Only {} properties specified in histogram configurationstring: {}", hProps_.size(), cfgStr);
  //        return false;
  //    }
  //

    return true;
}
