#ifndef GUARD_RootHandler_h
#define GUARD_RootHandler_h

/*------------------------------------------------------------------------------
  RootHandler Class

 Created : 2016-11-03  godshalk
 Modified: 2017-03-24  godshalk

 Class that manages the creation of a TFile, the creation of histograms within
 that file.

 Adapted from RootHandler from an earlier version of this code.



------------------------------------------------------------------------------*/

// Standard Libraries
#include <map>
#include <string>
// Root Classes
#include <TFile.h>
#include <TH1.h>
// Project Specific classes
#include "Logger.h"


class RootHandler
{
  public:
    RootHandler(std::string);
    ~RootHandler(){}

    TH1* createHistogram(std::string);   // Creates a histogram within the root file, based on a configuration string.
    TH1* getHist(std::string key){return histograms_[key];}
    void close();   // Saves and closes the root file.



  private:
    bool getHistoPropsFromString(const std::string&);   // Pulls histogram properties from the input string, stores in hProps_;

  // File information
    std::string fileName_;
    // std::string filePath_;
    TFile* file_;

    std::map<std::string, TH1*> histograms_;   // Map of histograms, keyed by their names.

    Logger logger_;
};

#endif
