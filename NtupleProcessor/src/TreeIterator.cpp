/*
TreeIterator.cpp

 Created : 2015-05-14  godshalk
 Modified: 2017-03-20  godshalk

 2016-10-24 - Moved constructor into source.
 2016-10-16 - Transferred, slightly modified to work with new version of
              NtupleProcessor

*/

#include <iostream>
#include "TreeIterator.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

TreeIterator::TreeIterator(vector<HistogramExtractor*>& vHE)
  : fChain(0), hExtractors_(vHE),
    nEntries_(0), finalEntry_(0),
    nEntriesProcessed_(0), logger_("NtupleProcessor", "[TI]", 1)
{
    logger_.debug("TreeIterator Created.");
  // Initialize EH and pass to HEs
    evt_ = new EventHandler();
    for( HistogramExtractor* h: hExtractors_ ) h->setEventHandler(evt_);
}

void TreeIterator::Begin(TTree * /*tree*/){}


void TreeIterator::SlaveBegin(TTree * /*tree*/)
{
    logger_.debug("TreeIterator::SlaveBegin().");
  // Initialize log, counters
    nEntries_          = 0;
    nEntriesProcessed_ = 0;
}


void TreeIterator::Init(TTree *tree)
{
  // Initialize the current tree.
    if (!tree) return;
    fChain = tree;              // Commented, don't think I need to access tree information outside of this function.
    fChain->SetMakeClass(1);    // ??? Can't seem to get a good answer about what this is.
                                //    I think it recreates classes stored in the root tree as opposed to just mapping variables from the object.
    nEntries_   = tree->GetEntries();
    finalEntry_ = tree->GetEntries()-1;

    logger_.info("Processing New Tree (# Entries: {})", nEntries_);

  // Initialize Event Handler, adding the criteria of each HistoMaker to it's list of criteria.
    evt_->mapTree(fChain);
}

Bool_t TreeIterator::Notify()
{
  //  Init(fChain);
    return kTRUE;
}

Bool_t TreeIterator::Process(Long64_t entry)
{
  // Load current entry
    fChain->GetTree()->GetEntry(entry);

  // Set up status outputs
    if(nEntriesProcessed_%100000 == 0 || nEntriesProcessed_==finalEntry_) cout << "  " << nEntriesProcessed_ << endl;

  // Evaluate selection profiles.
    evt_->evaluateEvent();

  // Call each HistogramExtractors
    for( HistogramExtractor* h: hExtractors_ ) h->process();

    nEntriesProcessed_++;

    return true;
}

void TreeIterator::SlaveTerminate()
{
    // for( HistogramExtractor* h: hExtractors_ ) h->terminate();
}

void TreeIterator::Terminate(){}
