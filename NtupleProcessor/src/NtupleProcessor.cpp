/*------------------------------------------------------------------------------
NtupleProcessor.cpp
 Created : 2015-09-14  godshalk
 Modified: 2017-03-20  godshalk
------------------------------------------------------------------------------*/

// Standard Libraries
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// Boost Libraries
#include <boost/property_tree/ptree.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
// ROOT Libraries
#include "TH1F.h"
// Project Specific classes
#include "NtupleProcessor.h"
#include "CutFlowTable.h"
#include "UtilFunctions.h"

using std::endl;   using std::vector    ;
using std::cout;   using std::string    ;
using std::cin ;   using std::shared_ptr;
using std::map ;   using std::to_string ;
using utilityFunctions::getListFromString;
typedef unsigned long counter;
typedef boost::property_tree::ptree propTree;
typedef propTree::const_iterator ptreeIter;
namespace po = boost::program_options;

// MAIN() - Used only to pass on input to NtupleProcessor class.
int main(int argc, char* argv[])
{ // Set up NtupleProcessor

  // Try/catch set up for the sole pupose of catching false return on
    try { NtupleProcessor nProc (argc, argv); }
    catch(const char* msg)
    {   if(string(msg) != "help") std::cerr << msg << endl;
        return 1;
    }
    return 0;
}

NtupleProcessor::NtupleProcessor(int argc, char* argv[])
  : ntupleLabel_(""), cfgFileName_("ZClibrary/config/ntupleprocessor/default.ini"),
    logQuiet_(false), logDebug_(false),
    eventsToProcess_(0), firstEventToProcess_(0), options_(""),
    logger_("NtupleProcessor", "[NP]", 0)
{ // Class initialization
    beginTime_.update();  // Set begin time.
    if(!processCommandLineInput(argc, argv)) throw("help"); // Process command line input. Throw error if unsucessful.

  // Initial output to logger, including timestamp.
    logger_.info("");
    logger_.info("================================================================================");
    logger_.info("===Initializing===");
    logger_.debug("NtupleProcessor Created.");

  // Initialize Helper Classes, histogram makers, ntuples.
    if( !initializeConfig() || !initializeNtuple() || !initializeHistogramExtractors() ) throw("help");
    tIter_ = new TreeIterator(hExtractors_);

    logger_.info("NtupleProcessor initizated {}", beginTime_.log_str());
    if(eventsToProcess_>0) logger_.info("Number of events to process: {}", eventsToProcess_);

  // Process the tree.
    processNtuple();

  // Terminate histogram extractors and print closing output.
    logger_.info("");
    for( HistogramExtractor* h: hExtractors_ ) h->terminate();

  // Print closing output.
    endTime_.update();
    logger_.info("");
    logger_.info("===NtupleProcessor complete {}===", endTime_.log_str());
    logger_.info("================================================================================");
}

bool NtupleProcessor::processCommandLineInput(int argc, char* argv[])
{ // Process command line input.
  // Returns false if help is called or input is invalid.
    logger_.trace("processCommandLineInput() called.");

  // Set up options
    po::options_description opDesc("NtupleProcessor options", 150);
    opDesc.add_options()
        ("help"      ",h",                                                             "Print help message"                                         )
        ("config"    ",c",  po::value<string>()->default_value(cfgFileName_         ), "Config file to use (from ZCLibrary/config/ntupleprocessor/)")
        ("ntuple"    ",n",  po::value<string>()->default_value(ntupleLabel_         ), "Ntuple to process (from cfg in ZCLibrary/config/ntuple/)"   )
        ("maxevents" ",m",  po::value<int>()   ->default_value(eventsToProcess_     ), "Number of events to process"                                )
        ("firstevent"",f",  po::value<int>()   ->default_value(firstEventToProcess_ ), "Number of events to process"                                )
        ("debug"     ",d",                                                             "Increased output for debugging purposes."                   )
        ("quiet"     ",q",                                                             "Minimal console output. Output still logged in file."       )
        ("options"   ",o",  po::value<string>()->default_value(options_             ), "Misc. options"                                              )
    ;
    po::variables_map cmdInput;
    po::store(po::parse_command_line(argc, argv, opDesc), cmdInput);
    po::notify(cmdInput);
    if(cmdInput.count("help"))
    {   cout << "\n" << opDesc << endl; return false; }

  // Extract and store variables.
    cfgFileName_         = ( cmdInput.count("config") ? cmdInput["config"].as<string>() : "ZClibrary/config/ntupleprocessor/default.ini");
    ntupleLabel_         = ( cmdInput.count("ntuple") ? cmdInput["ntuple"].as<string>() : "");
    logDebug_            =   cmdInput.count("debug");
    logQuiet_            =   cmdInput.count("quiet");
    eventsToProcess_     = ( cmdInput.count("maxevents" ) ? cmdInput["maxevents" ].as<int>() :  0);
    firstEventToProcess_ = ( cmdInput.count("firstevent") ? cmdInput["firstevent"].as<int>() :  0);

  // Log files will be huge if debug and no max event setting are set. Give a warning.
    if(logDebug_ && eventsToProcess_ > 1000 )
    {   string debugQuestion = "\n  WARNING: You are about to run NP with the debug option set over more than 1000 events."
                                           " The output log file will be large (>>MB). Would you like to continue?";
        if(!getUserYNInput(debugQuestion))
        { cout << "\nAborting program.\n" << endl; return false; }
    }

  // Set up logger with quiet, debug info.
    if(logDebug_)
    {   logger_.setFormat("[%Y-%m-%d %H:%M:%S.%e|%l]%v");
        logger_.setDebug(true);
    }
    if(logQuiet_) logger_.setQuiet(true);
    return true;
}

bool NtupleProcessor::initializeConfig()
{ // Loads main NP config from command line input, as well as other configs
  // specified in the NP config
    logger_.debug("initializeConfig(): Getting config file {}", cfgFileName_);
  // Load NP config file.
    procCfg_             = cfgLocator_.getConfig(cfgFileName_);
    configPath_          =                             procCfg_->get<string>("config_dir");
    ntupleCfgName_       = configPath_ + "ntuple/"   + procCfg_->get<string>("ntuple"    );
    dsCfgName_           = configPath_ + "dataset/"  + procCfg_->get<string>("dataset"   );
    eventMapCfgName_     = configPath_ + "eventmap/" + procCfg_->get<string>("eventmap"  );
    ntupleCfg_           = cfgLocator_.getConfig(ntupleCfgName_);
    datasetCfg_          = cfgLocator_.getConfig(dsCfgName_    );
  // Set up global instance of event map configuration.
    eventMapInstanceCfg_ = cfgLocator_.setConfig("current_event_map", eventMapCfgName_);
    return true;
}

bool NtupleProcessor::initializeNtuple()
{ // Sets up an ntuple from the input ntuple label.
  // Returns false if the ntuple is not found in the given configuration files.
  // TO DO: Make NtupleInfo into its own class/struct, so it's easier to keep track of what info goes into the config file.
  //   (Easier to look at a class definition rather than checking this function to see what fields are defined for this config file.)
    logger_.trace("initializeNtuple(): Called.");

  // Extract ntuple information from ntuple config file.
    string ntuplePath    = ntupleCfg_->get<string>("ntuple_path" );
    string rawNtupleInfo = ntupleCfg_->get<string>( ntupleLabel_ );
    logger_.debug("initializeNtuple(): from ntuple cfg for label {}: {}", ntupleLabel_, rawNtupleInfo);
    if(rawNtupleInfo == "")
    {   logger_.error("No information for {} stored in ntuple config file {}", ntupleLabel_, ntupleCfgName_);
        return false;
    }
    // vector<string> splitInfo;   ConfigReader::getListFromString(rawNtupleInfo, splitInfo);
    vector<string> splitInfo;   getListFromString(rawNtupleInfo, splitInfo);
    // Info has format in file: <label> = <dataset> <dataset_property_list> <filename>

  // Add information for ntuple to a map by iterating through split list.
    map<string, string> ntupleInfo;
    ntupleInfo["label"      ] = ntupleLabel_;
    auto nInfo = splitInfo.begin();
    ntupleInfo["data_or_sim"] = *nInfo++;
    ntupleInfo["dataset"    ] = *nInfo++;
    ntupleInfo["properties" ] = *nInfo++;
    while(nInfo != splitInfo.end()-1)
        ntupleInfo["properties"] += " " + *nInfo++;
    ntupleInfo["filename"  ] = ntuplePath + '/' + *nInfo++;

  // Use extracted info on file to open the root file
    ntupleFile_ = TFile::Open(ntupleInfo["filename"].c_str());

  // Extract tree pointer and count info from file.
    ntupleTree_ = (TTree*) ntupleFile_->Get("tree");
    long posCounts = ((TH1F*) ntupleFile_->Get("CountPosWeight"))->GetBinContent(1);
    long negCounts = ((TH1F*) ntupleFile_->Get("CountNegWeight"))->GetBinContent(1);
    long absCounts = ((TH1F*) ntupleFile_->Get("Count"         ))->GetBinContent(1);
    long treeEntries = ntupleTree_->GetEntries();
    ntupleInfo["abs_counts"  ] = to_string(absCounts);
    ntupleInfo["pos_counts"  ] = to_string(posCounts);
    ntupleInfo["neg_counts"  ] = to_string(negCounts);
    ntupleInfo["net_counts"  ] = to_string(posCounts-negCounts);
    ntupleInfo["tree_entries"] = to_string(treeEntries);

  // Store the mapped information in a config file for this NP instance. Get a pointer to it for later.
    ntupleInstanceCfg_ = cfgLocator_.setConfig("current_ntuple_info", ntupleInfo);

    // NtupleInfo* ntupleInfoSingleton = NtupleInfo::getInstance();
    NtupleInfo::getInstance();

    return true;
}

bool NtupleProcessor::initializeHistogramExtractors()
{ // Sets up HEs using NP config file.
    logger_.debug("initializeHistogramExtractors(): called.");

// TEST: Check extraction of information procCfg_
    // logger_.debug("Attempting to pull HISTOGRAM EXTRACTORS tree from ntupleprocessor cfg.");
    // propTree heCfgStrs = procCfg_->getSubTree("HISTOGRAM EXTRACTORS");
    // logger_.debug("Config strings stored in HISTOGRAM EXTRACTORS:");
    // for(const auto& heStr : heCfgStrs)
    //     logger_.debug("-> {} = {}", heStr.first, string(heStr.second.data()));

  // Get HE properties from ntupleprocessor config file.
    propTree heCfgStrs = procCfg_->getSubTree("HISTOGRAM EXTRACTORS");
    // hExtractors_.push_back(new CutFlowTable());
    // hExtractors_.push_back(HistogramExtractor::generateHistogramExtractor("CutFlowTable SELECTION_STR"));
    // hExtractors_.push_back(HistogramExtractor::generateHistogramExtractor(procCfg_->get<string>("HISTOGRAM EXTRACTORS.cft")));
    for(const auto& heStr : heCfgStrs)
    {   logger_.debug("Creating histogram from string: {}", heStr.second.data());
        hExtractors_.push_back(HistogramExtractor::generateHistogramExtractor(heStr.second.data()));
    }
    return true;
}

void NtupleProcessor::processNtuple()
{ // Takes the input ntuple and calls the tree iterator over it.
    logger_.trace("processNtuple() called.");
    logger_.info("");
    logger_.info("================================================================================");
    logger_.info("===Beginning Event Processing===");

    if( eventsToProcess_ > 0 || firstEventToProcess_ > 0 )
    {   if( eventsToProcess_ == 0 ) eventsToProcess_ = ntupleInstanceCfg_->get<int>("tree_entries");
        ntupleTree_->Process(tIter_, "", eventsToProcess_, firstEventToProcess_);
    }
    else ntupleTree_->Process(tIter_);

    logger_.info("");
    logger_.info("===End Event Processing===");
    logger_.info("================================================================================");
}

bool NtupleProcessor::getUserYNInput(std::string query)
{ // Get a yes or no answer from the user,
    cout << query << " [y/N]: ";
    char a[8];
    cin.getline(a, 8);
    string answer = a;
    if(answer != "y" && answer != "Y" && answer != "yes" && answer != "Yes" && answer != "YES" )
        return false;
    return true;
}
