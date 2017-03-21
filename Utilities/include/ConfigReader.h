#ifndef GUARD_ConfigReader_h
#define GUARD_ConfigReader_h
/*------------------------------------------------------------------------------
   ConfigReader

 Created : 2015-10-28 godshalk
 Modified: 2017-03-20  godshalk

 Class that handles the input of a single configuration file.

 To do:
 - Implement base config retrieval, reference
 - Think about relation to ConfigLocator
    - Should CL just have a shared_ptr list w/ all CR files?
    - Should CL take care of creating, distributing CRs?
    - All this and more on the next exciting episode of Andrew continues to overthink his code.

------------------------------------------------------------------------------*/

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Logger.h"
// #include "ConfigLocator.h"

class ConfigReader {
  public:
    ConfigReader()
    : base_(NULL),
      fn_config_(""),
      logger_("NtupleProcessor", "[CR]", 3)
    {} // Blank constructor.

    ConfigReader(const std::string& fnc)
    : base_(NULL),
      fn_config_(fnc),
      logger_("NtupleProcessor", "[CR]", 3)
    {   logger_.trace("Creating CR from {}", fn_config_);
      // Open and read in config file
        boost::property_tree::ini_parser::read_ini(fn_config_.c_str(), pt);
      // Check file for a base config file (i.e. a file that this config is built off of.)
      // Set pointer to base if found.
        // if(pt.find("base") != pt.not_found())
        //     base_ = ConfigHandler::getConfig(pt.get<std::string>("base"));
    }

    ~ConfigReader(){}

    template <typename T>
    T get(const std::string& key) const
    { // If branch doesn't exist, return the default object.
        if(pt.find(key) == pt.not_found()) return T();
        return pt.get<T>(key);
    }

    template <typename T>
    void get(const std::string& key, T& data) const
    { // Another option for retrieving data, this time passing the object to write to.
        data = get<T>(key);
    }

    template <typename T>
    void getListFromString(std::string& str, std::vector<T>& list)
    { // Simple function that extracts the templated object from a string.
      // Feeds string into a stringstream and, while there is still something to
      //   read out, ouputs the entry from the stream into an variable and
      //   pushes the variable into the output vector.
        list.clear();
        std::stringstream strm(str);
        while(true) {
            T n;   strm >> n;
            if(!strm) break;
            list.push_back(n);
        }
    }

  protected:
    ConfigReader* base_     ;
    std::string   fn_config_;         // Location of the input configuration file.
    boost::property_tree::ptree pt;   // Property tree read from file.
    Logger logger_;
};

#endif
