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
#include <vector>
// #include "Logger.h"
// #include "ConfigLocator.h"

// Forward declarations
class ConfigReader;
class ConfigLocator;
typedef std::shared_ptr<ConfigReader> ConfigPtr;
typedef boost::property_tree::ptree propTree;
typedef propTree::const_iterator ptreeIter;

class ConfigReader {
  public:
    // ConfigReader();
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

    // Functions passed on from property tree
    ptreeIter begin()               const {return pt.begin()  ;}
    ptreeIter end()                 const {return pt.end()    ;}
    // ptreeIter find(std::string key) const {return pt.find(key);}
    const propTree getInfoTree() const {return pt;}
    const propTree getSubTree(std::string key) const
    {
        // return pt.get_child(key);
        return (pt.find(key) == pt.not_found() ? base_->getSubTree(key) : pt.get_child(key) );
    }

  protected:
    ConfigPtr base_ ;  // Base config that this config modifies.
        // Refers to this pointer for values that aren't stored in this config.
    std::string fn_config_;           // Location of the input configuration file.
    propTree pt;   // Property tree read from file.
    static ConfigLocator* cfgLocator_;
    // Logger logger_;
};


/*------------------------------------------------------------------------------
   ConfigLocator

 Class that loosely follows service locator pattern, meant to load configuration
 files and share them across multiple classes.

------------------------------------------------------------------------------*/

class ConfigLocator {

protected:
  static std::map<std::string, ConfigPtr > cfgs_;
  // Logger logger_;

  public:
    // ConfigLocator(){}
    // ~ConfigLocator(){}

    static ConfigPtr getConfig(std::string cfg_name)
    { // If configuration hasn't already been loaded, load it.
        if(cfgs_.find(cfg_name) == cfgs_.end()) cfgs_[cfg_name] = std::make_shared<ConfigReader>(cfg_name);
        return cfgs_[cfg_name];
    }

    static ConfigPtr setConfig(std::string cfg_label, std::string cfg_name)
    { // Set configuration given to a general label for access across classes.
        return cfgs_[cfg_label] = getConfig(cfg_name);
    }

    static ConfigPtr setConfig(std::string cfg_label, const std::map<std::string, std::string>& cfg_map)
    { // Set configuration given to a general label for access across classes.
        return cfgs_[cfg_label] = std::make_shared<ConfigReader>(cfg_map);
    }

    static bool configLoaded(std::string cfg_name)
    { // Returns if the config has been created.
        return cfgs_.find(cfg_name) == cfgs_.end();
    }

};

#endif
