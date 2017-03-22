#ifndef GUARD_EventMap_h
#define GUARD_EventMap_h

/*------------------------------------------------------------------------------
   EventMap

 Created : 2016-10-25  godshalk
 Modified: 2017-03-22  godshalk

 Maps event values in a TTree into structs/classes/variables,
 which are then accessed by EventHandler.

------------------------------------------------------------------------------*/

// Standard Libraries
#include <map>
#include <memory>
#include <string>
#include <vector>
// Root Classes
#include <TChain.h>
// Project Specific classes
#include "ConfigReader.h"
#include "Logger.h"

class EventMap
{
  public:
    EventMap();
    ~EventMap(){}

    const static int maxNparticles = 100;   // WORKING VARIABLE - include in config file at some point?

    void mapTree(TTree*);

    // Map variables
    std::map<std::string, float               > mf_ ;
    std::map<std::string, int                 > mi_ ;
    std::map<std::string, float[maxNparticles]> mfa_;
    std::map<std::string, int  [maxNparticles]> mia_;

  private:
    Logger logger_;
    ConfigLocator cfgLocator_;
    std::shared_ptr<ConfigReader> evtMapList_;
};

#endif
