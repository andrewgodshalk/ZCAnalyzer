#ifndef GUARD_TreeIterator_h
#define GUARD_TreeIterator_h

/*------------------------------------------------------------------------------
   TreeIterator

 Created : 2015-05-14  godshalk
 Modified: 2017-03-20  godshalk

 Class inheritted from TSelector (from ROOT libraries). TSelector implementation
 documentation can be found at https://root.cern.ch/developing-tselector.

 Created to run over (chains of) Z+c Ntuple trees.

 2016-11-03 - Moved HistogramExtractor creation, termination to NtupleProcessor.
 2016-10-24 - Integrated global logger. Moved constructor into source.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <memory>
#include <vector>
// Root Classes
#include <TTree.h>
#include <TSelector.h>
// Project Specific classes
#include "EventHandler.h"
#include "HistogramExtractor.h"
#include "Logger.h"

typedef unsigned long counter;

class TreeIterator : public TSelector
{
  public:
    TreeIterator(std::vector<HistogramExtractor*>&);
    // TreeIterator();
    virtual ~TreeIterator(){}

  // Overloaded TSelector Functions
    virtual Int_t   Version() const { return 2; }
    virtual void    Begin     (TTree *tree);
      // Run at the client. Runs commands that access information at client location.
    virtual void    SlaveBegin(TTree *tree);
      // Run at the client or at worker. All initialization that will be needed
      //   for Process() should be worked out here.
    virtual void    Init      (TTree *tree);
      // Called when initizlizing a new tree/chain. Branch addresses set here.
    virtual Bool_t  Notify();
      // Called when opening a new file. Branch pointers retrieved here.
    virtual Bool_t  Process   (Long64_t entry);
      // Called for each entry processed from chain. Workhorse function.
    virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
    virtual void    SetOption(const char *option) { fOption = option; }
    virtual void    SetObject(TObject *obj)       { fObject = obj;    }
    virtual void    SetInputList(TList *input)    { fInput  = input;  }
      // Set input objects to be transferred to workers. Called btwn Begin and SlaveBegin.
    virtual TList  *GetOutputList() const         { return fOutput;   }
      // Get output objects from workers. Objects combined by PROOF master server.
      //   Called btwn SlaveTerminate and Terminate.
    //virtual const char *GetOption();
      // Gets option that is passed to chain->Process(). Not overloaded.
    virtual void    SlaveTerminate();
    virtual void    Terminate();
      // Worker and client termination functions.

  private:
    TTree *fChain;
    EventHandler* evt_;
    std::vector<HistogramExtractor*> hExtractors_;

  // Internal counters
    counter nEntries_;            // Total number of entries in chain
    counter finalEntry_;          // Index of last entry
    counter nEntriesProcessed_;   // Total number of events processed from chain

    Logger logger_;


};


#endif
