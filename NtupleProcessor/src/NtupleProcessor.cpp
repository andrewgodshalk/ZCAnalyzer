/*------------------------------------------------------------------------------
NtupleProcessor.cpp
 Created : 2015-09-14  godshalk
 Modified: 2017-03-20  godshalk
------------------------------------------------------------------------------*/

// Standard Libraries
#include <iostream>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// Boost Libraries
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
// ROOT Libraries
#include "TROOT.h"
#include "TApplication.h"
#include "TRint.h"
// Project Specific classes
#include "NtupleProcessor.h"

using std::endl;   using std::vector;
using std::cout;   using std::string;
using std::cin ;
typedef unsigned long counter;
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
  : logQuiet_(false), logDebug_(false), eventsToProcess_(-1), firstEventToProcess_(0), options_(""), logger_("NtupleProcessor", "[NP]", 0)
{ // Class initialization
    beginTime_.update();  // Set begin time.
    if(!processCommandLineInput(argc, argv)) throw("help"); // Process command line input. Throw error if unsucessful.

  // Initial output to logger, including timestamp.
    logger_.info("");
    logger_.info("================================================================================");
    logger_.info("===Initializing===");
    logger_.debug("NtupleProcessor Created.");

  // Initialize Helper Classes, histogram makers, ntuples.
    // initializeNtuples();
    // initializeHistogramExtractors();
    // tIter_ = new TreeIterator(hExtractors_);
    tIter_ = new TreeIterator();
    logger_.info("NtupleProcessor initizated {}", beginTime_.log_str());
    if(eventsToProcess_>0) logger_.info("Number of events to process: {}", eventsToProcess_);

  // TEMP TEST: Initialize and extract values from a ConfigReader.
    string fn_cfg = "/home/godshalk/Work/2017-03_ZCAnalyzer/ZCLibrary/config/ntupleprocessor/default.ini";
    ConfigReader* cfg = new ConfigReader(fn_cfg);
    logger_.debug("From cfg: test_line = {}", cfg->get<string>("test_line"));
    string ntupleFile; cfg->get("ntuple", ntupleFile);
    logger_.debug("From cfg: ntuple = {}", ntupleFile);
    logger_.debug("From cfg: LISTINGTHATISNOTTHERE = {}", cfg->get<string>("LISTINGTHATISNOTTHERE"));

    logger_.info("NtupleProcessor initizated {}", beginTime_.log_str());
    if(eventsToProcess_>0) logger_.info("Number of events to process: {}", eventsToProcess_);

  // Process the tree.
    processNtuple();

  // Terminate histogram extractors and print closing output.
    logger_.info("");
    // for( HistogramExtractor* h: hExtractors_ ) h->terminate();

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
        ("help"      ",h",                                                             "Print help message"                                   )
        ("debug"     ",d",                                                             "Increased output for debugging purposes."             )
        ("quiet"     ",q",                                                             "Minimal console output. Output still logged in file." )
        ("maxevents" ",m",  po::value<int>()   ->default_value(eventsToProcess_     ), "Number of events to process"                          )
        ("firstevent"",f",  po::value<int>()   ->default_value(firstEventToProcess_ ), "Number of events to process"                          )
        ("options"   ",o",  po::value<string>()->default_value(options_             ), "Misc. options"                                        )
    ;
    po::variables_map cmdInput;
    po::store(po::parse_command_line(argc, argv, opDesc), cmdInput);
    po::notify(cmdInput);
    if(cmdInput.count("help"))
    {   cout << "\n" << opDesc << endl; return false; }

  // Extract and store variables.
    logDebug_            =   cmdInput.count("debug");
    logQuiet_            =   cmdInput.count("quiet");
    eventsToProcess_     = ( cmdInput.count("maxevents" ) ? cmdInput["maxevents" ].as<int>() : -1);
    firstEventToProcess_ = ( cmdInput.count("firstevent") ? cmdInput["firstevent"].as<int>() : -1);

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

void NtupleProcessor::processNtuple()
{ // Takes the input ntuple and calls the tree iterator over it.
    logger_.trace("processNtuple() called.");
    logger_.info("");
    logger_.info("================================================================================");
    logger_.info("===Beginning Event Processing===");

    // TEMP TEST: Open hard-coded file, run over tree.
    string fn = "file:///home/godshalk/Work/2017-02-27_SampleNtuplesRR/MC_SUMMER2016_PRIMARY_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-Py8__RunIISummer16MAv2-PUMoriond17_80r2as_2016_TrancheIV_v6_ext2-v1_52of53.root";
    TFile* f = TFile::Open(fn.c_str());
    // if(!f->IsOpen()) logger_.debug("File not open: {}", fn);
    // logger_.debug("Opening file: {}", f->GetName());

    TTree* tree = (TTree*) f->Get("tree");
    // logger_.debug("Opening tree: {}", tree->GetTitle());

    if( eventsToProcess_ > 0 || firstEventToProcess_ > 0 ) tree->Process(tIter_, "", eventsToProcess_, firstEventToProcess_);
    else tree->Process(tIter_);

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
