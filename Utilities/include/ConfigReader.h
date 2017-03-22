#ifndef GUARD_ConfigReader_h
#define GUARD_ConfigReader_h
/*------------------------------------------------------------------------------
   ConfigReader & ConfigLocator

 Created : 2015-10-28 godshalk
 Modified: 2017-03-20 godshalk

 Class that handles the input of a single configuration file.

 To do:
 - Figure out how to use relative directories.
    - For example, how config/ntupleprocessor/cujo.ini would know to look for its default file in "config/ntupleprocessor"
    - Is there a way for the executabe to know its calling directory?
    - Would it be better to add working directories to an ini somewhere?
    - In NP, defininig a default path relative to repo main dir (i.e. ZCLibrary/config/ntupleprocessor/default.ini)
      or even just a similar file path, or a separate config, might work...

------------------------------------------------------------------------------*/

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "Logger.h"
// #include "ConfigLocator.h"

// Forward declarations
class ConfigLocator;


class ConfigReader {
  public:
    ConfigReader();
    ConfigReader(const std::string&);
    ConfigReader(const std::map<std::string, std::string>&);
    ~ConfigReader(){}

    template <typename T>
    T get(const std::string& key) const
    { // If key is not listed...
        if(pt.find(key) == pt.not_found())
        {   if(base_) return base_->get<T>(key);  // Check the default config
            else return T();  // Return the default constructor for this type.
        }
        return pt.get<T>(key);
    }

    template <typename T>
    void get(const std::string& key, T& data) const
    { // Another option for retrieving data, this time passing the object to write to.
        data = get<T>(key);
    }

    template <typename T>
    static void getListFromString(std::string& str, std::vector<T>& list)
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
    std::shared_ptr<ConfigReader> base_ ;  // Base config that this config modifies.
        // Refers to this pointer for values that aren't stored in this config.
    std::string fn_config_;           // Location of the input configuration file.
    boost::property_tree::ptree pt;   // Property tree read from file.
    static ConfigLocator* cfgLocator_;
    Logger logger_;
};



/*------------------------------------------------------------------------------
   ConfigLocator

 Class that loosely follows service locator pattern, meant to load configuration
 files and share them across multiple classes.

------------------------------------------------------------------------------*/

class ConfigLocator {
  public:
    ConfigLocator(){}
    ~ConfigLocator(){}

    static std::shared_ptr<ConfigReader> getConfig(std::string cfg_name)
    { // If configuration hasn't already been loaded, load it.
        if(cfgs_.find(cfg_name) == cfgs_.end()) cfgs_[cfg_name] = std::make_shared<ConfigReader>(cfg_name);
        return cfgs_[cfg_name];
    }

    static std::shared_ptr<ConfigReader> setConfig(std::string cfg_label, std::string cfg_name)
    { // Set configuration given to a general label for access across classes.
        return cfgs_[cfg_label] = getConfig(cfg_name);
    }

    static std::shared_ptr<ConfigReader> setConfig(std::string cfg_label, const std::map<std::string, std::string>& cfg_map)
    { // Set configuration given to a general label for access across classes.
        return cfgs_[cfg_label] = std::make_shared<ConfigReader>(cfg_map);
    }

  protected:
    static std::map<std::string, std::shared_ptr<ConfigReader> > cfgs_;
    // Logger logger_;
};

std::map<std::string, std::shared_ptr<ConfigReader> > ConfigLocator::cfgs_;


// =============================================================================
// === ConfigReader member functions

ConfigLocator* ConfigReader::cfgLocator_ = new ConfigLocator();

ConfigReader::ConfigReader()
: base_(NULL),
  fn_config_(""),
  logger_("NtupleProcessor", "[CR]", 3)
{} // Blank constructor.

ConfigReader::ConfigReader(const std::string& fnc)
: base_(NULL),
  fn_config_(fnc),
  logger_("NtupleProcessor", "[CR]", 3)
{   logger_.trace("Creating CR from {}", fn_config_);
  // Open and read in config file
    boost::property_tree::ini_parser::read_ini(fn_config_.c_str(), pt);
  // Check file for a base config file (i.e. a file that this config is built off of.)
    if(pt.find("base") != pt.not_found())
      // Set pointer to base if found.
        base_ = cfgLocator_->getConfig(pt.get<std::string>("base"));
}

ConfigReader::ConfigReader(const std::map<std::string, std::string>& input_map)
: base_(NULL),
  fn_config_(""),
  logger_("NtupleProcessor", "[CR]", 3)
{   logger_.trace("Creating CR from {}", fn_config_);
  // Copy map keys, values into new ptree.
    // pt = boost::property_tree::basic_ptree();
    for( const auto& kv : input_map)
        pt.push_back(std::make_pair(kv.first, boost::property_tree::ptree(kv.second)));
  // Check file for a base config file (i.e. a file that this config is built off of.)
    if(pt.find("base") != pt.not_found())
      // Set pointer to base if found.
        base_ = cfgLocator_->getConfig(pt.get<std::string>("base"));
}

#endif
