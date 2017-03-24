
// #include "../include/ConfigReader.h"
#include "ConfigReader.h"

using std::map;
using std::shared_ptr;
using std::string;

map<string, ConfigPtr > ConfigLocator::cfgs_ = map<string, ConfigPtr >();

ConfigLocator* ConfigReader::cfgLocator_ = new ConfigLocator();

// ConfigReader::ConfigReader()
// : base_(NULL),
//   fn_config_("")
//   // logger_("NtupleProcessor", "[CR]", 3)
// {} // Blank constructor.

ConfigReader::ConfigReader(const std::string& fnc)
: base_(NULL),
  fn_config_(fnc)
  // logger_("NtupleProcessor", "[CR]", 3)
{  // logger_.trace("Creating CR from {}", fn_config_);
  // Open and read in config file
    boost::property_tree::ini_parser::read_ini(fn_config_.c_str(), pt);
  // Check file for a base config file (i.e. a file that this config is built off of.)
    if(pt.find("base") != pt.not_found())
      // Set pointer to base if found.
        base_ = cfgLocator_->getConfig(pt.get<std::string>("base"));
}

ConfigReader::ConfigReader(const std::map<std::string, std::string>& input_map)
: base_(NULL),
  fn_config_("")
  // logger_("NtupleProcessor", "[CR]", 3)
{  // logger_.trace("Creating CR from {}", fn_config_);
  // Copy map keys, values into new ptree.
    // pt = boost::property_tree::basic_ptree();
    for( const auto& kv : input_map)
        pt.push_back(std::make_pair(kv.first, boost::property_tree::ptree(kv.second)));
  // Check file for a base config file (i.e. a file that this config is built off of.)
    if(pt.find("base") != pt.not_found())
      // Set pointer to base if found.
        base_ = cfgLocator_->getConfig(pt.get<std::string>("base"));
}
