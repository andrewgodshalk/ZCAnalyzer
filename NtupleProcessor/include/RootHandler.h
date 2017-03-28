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

    void addHistogram(const std::string, TH1*);   // Adds histogram to this RH
    TH1* createHistogram( const std::string, const std::vector<std::string>&);   // Creates a histogram within the root file, based on a key and configuration string.
    TH1* getHist(const std::string key) const {return histograms_.at(key);}
    // bool cd(const std::string path=""){return file_->cd(path.c_str())}
    void close();   // Saves and closes the root file.

  private:
  // File information
    std::string fileName_;
    TFile* file_;

    std::map<const std::string, TH1*> histograms_;   // Map of histograms, keyed by their names.

    Logger logger_;
};

#endif
