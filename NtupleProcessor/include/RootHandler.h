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

    void addHistogram(TH1*, std::string);   // Adds histogram to this RH
    TH1* createHistogram(   std::string);   // Creates a histogram within the root file, based on a configuration string.
    TH1* getHist(std::string key){return histograms_[key];}
    // bool cd(const std::string path=""){return file_->cd(path.c_str())}
    void close();   // Saves and closes the root file.

  private:
    bool getHistoPropsFromString(const std::string&);   // Pulls histogram properties from the input string, stores in hProps_;

  // File information
    std::string fileName_;
    // std::string filePath_;
    TFile* file_;

    std::map<std::string, TH1*> histograms_;   // Map of histograms, keyed by their names.
    std::vector<std::string> hProps_;          // Vector initialized at runtime to store histogram properties read in from a configuration string.

    Logger logger_;
};

#endif
