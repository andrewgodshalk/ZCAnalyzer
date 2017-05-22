#ifndef GUARD_LeptonObject_h
#define GUARD_LeptonObject_h

/*------------------------------------------------------------------------------
   LeptonObject

   Modified: 2017-04-28  godshalk
   Created : 2017-04-28  godshalk

------------------------------------------------------------------------------*/

// Standard Libraries
#include <map>
#include <string>

class LeptonObject
{
  public:
    LeptonObject(){}
    ~LeptonObject(){}

    float get(const std::string key) const {return properties_.at(key);}

  private:
    std::map<std::string, float> properties_;
};


#endif
