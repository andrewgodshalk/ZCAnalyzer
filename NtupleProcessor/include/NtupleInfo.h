#ifndef GUARD_NtupleInfo_h
#define GUARD_NtupleInfo_h
/*------------------------------------------------------------------------------
   NtupleInfo

 Created : 2017-03-24 godshalk
 Modified: 2017-03-24 godshalk

 Simplified singleton interface with runtime-defined "current_ntuple_info"
 configuration.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <string>
// ROOT Classes
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
// Project Specific Classes
#include "ConfigReader.h"
#include "Logger.h"


struct NtupleInfo
{
  private:
    ConfigLocator cfgLocator_;
    ConfigPtr currentNtupleInfo_;

  public:
  // Ntuple info
    const std::string label       ;
    const std::string dataset     ;
    const std::string properties  ;
    const std::string filename    ;
    const        bool isData      ;
    const        bool isSim       ;
    const        long ntupleEvents;   // Events in this ntuple.
    const        long absDSEvents ;   // Original number of events ntupler analyzed.
    const        long posDSEvents ;
    const        long negDSEvents ;
    const        long netDSEvents ;

  private:
    Logger logger_;

    static NtupleInfo* theInstance_;

    NtupleInfo() :
      currentNtupleInfo_(cfgLocator_.getConfig("current_ntuple_info"  )),
      label       (currentNtupleInfo_->get<std::string>("label"       )),
      dataset     (currentNtupleInfo_->get<std::string>("dataset"     )),
      properties  (currentNtupleInfo_->get<std::string>("properties"  )),
      filename    (currentNtupleInfo_->get<std::string>("filename"    )),
      isData      (currentNtupleInfo_->get<std::string>("data_or_sim" ) == "data"),
      isSim       (currentNtupleInfo_->get<std::string>("data_or_sim" ) == "sim" ),
      ntupleEvents(currentNtupleInfo_->get<long>("tree_entries")),
      absDSEvents (currentNtupleInfo_->get<long>("abs_counts"  )),
      posDSEvents (currentNtupleInfo_->get<long>("pos_counts"  )),
      negDSEvents (currentNtupleInfo_->get<long>("neg_counts"  )),
      netDSEvents (currentNtupleInfo_->get<long>("net_counts"  )),
      logger_("NtupleProcessor", "[NI]", 2)
    { // Instantiate NtupleInfo from "current_ntuple_info" config.
        logger_.debug(" The Instance Was Created!!");
        logger_.debug(" Ntuple Label: {}", currentNtupleInfo_->get<std::string>("label"));
        logger_.debug(" Ntuple Label: {}", label);
    }
    // NtupleInfo(const NtupleInfo & source ) : logger_("bleh", "Bleh!"){/* Disabling copy-ctor */}
    // NtupleInfo(      NtupleInfo && source) : logger_("bleh", "Bleh!"){/* Disabling move-ctor */}

  public:
    static NtupleInfo * getInstance()
    { // Instantiates NtupleInfo if it hasn't already.
        if(theInstance_ == NULL) theInstance_ = new NtupleInfo();
        return theInstance_;
    }

};


#endif
