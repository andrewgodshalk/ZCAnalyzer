
// #include "../include/ConfigReader.h"
#include "ConfigReader.h"
#include <iostream>

using std::map;
using std::shared_ptr;
using std::string;
using std::cout;
using std::endl;
using std::make_pair;

map<string, ConfigPtr > ConfigLocator::cfgs_ = map<string, ConfigPtr >();
// Logger ConfigLocator::logger_ = Logger("NtupleProcessor", "[CL]", 2);

ConfigLocator* ConfigReader::cfgLocator_ = new ConfigLocator();

// CR created with a filename, pointing to an INI config file.
ConfigReader::ConfigReader(const std::string& fnc)
: base_(NULL),
  fn_config_(fnc),
  logger_("NtupleProcessor", "[CR]", 3)
{ logger_.trace("Creating CR from {}", fn_config_);
  // Open and read in config file
    boost::property_tree::ini_parser::read_ini(fn_config_.c_str(), pt_);
  // Check file for a base config file (i.e. a file that this config is built off of.)
    initializeFromBase();
}

// CR created by program from a map<string,string>, created at runtime in the analyzer.
ConfigReader::ConfigReader(const std::map<std::string, std::string>& input_map)
: base_(NULL),
  fn_config_("CFG_FROM_MAP"),
  logger_("NtupleProcessor", "[CR]", 3)
{ logger_.trace("Creating CR from input map.");
  // Copy map keys, values into new ptree.
    for( const auto& kv : input_map)
        pt_.push_back(std::make_pair(kv.first, propTree(kv.second)));
  // Check file for a base config file (i.e. a file that this config is built off of.)
    initializeFromBase();
}

// Sets up the config from a base tree, if a "base" value is given.
void ConfigReader::initializeFromBase()
{
    if(pt_.find("base") != pt_.not_found())
    {   base_ = cfgLocator_->getConfig(pt_.get<string>("base"));   // Set pointer to base if found.
      // Create new tree with base values overloaded.
        logger_.debug("Overloading values in base tree: {} <-- {}", pt_.get<string>("base"), fn_config_);
        pt_ = overloadPropTree(base_->getInfoTree(), pt_);
        printTree(pt_);
    }
}

// Creates a new tree from the base tree with some values replaced by those in the overload tree.
propTree ConfigReader::overloadPropTree(const propTree& baseTree, const propTree& overloadTree)
{
  // Set up a new tree equal to the base tree.
    propTree newTree = baseTree;

  // Cycle through values in the overloadTree.
    ptreeIter overloadIter = overloadTree.begin();
    ptreeIter overloadEnd  = overloadTree.end()  ;
    for(; overloadIter != overloadEnd; overloadIter++)
    {
        std::string key = overloadIter->first;
        if(key == "base")
        {   logger_.debug("Skipping base key: {}", key);
            continue;
        }
        else logger_.debug("Processing key: {}", key);

      // Check if the value exists in the base tree. If not, throw/warn of error.
        if(newTree.find(key) == newTree.not_found())
            logger_.warn("Key from overloading tree not found in base: {}", key);

      // If the key points to a tree, recursively copy.
        // Referenced http://stackoverflow.com/questions/15195085/checking-if-a-value-in-a-boost-property-tree-is-a-tree-or-a-terminal-value
        if( !overloadIter->second.empty() && overloadIter->second.data().empty() )   // i.e. if the node isn't empty but there's no direct data...
        {   logger_.debug("Overloading subtree {}", key);
            newTree.put_child(key, overloadPropTree(baseTree.get_child(key), overloadTree.get_child(key)));
        }

      // Otherwise, just set the node value to that in the overloaded tree.
        else
        {   string treeData = overloadIter->second.data();
            if(newTree.find(key) == newTree.not_found())
            {   logger_.debug("Adding key {}: {}", key, treeData);
                newTree.add(key, treeData);
            }
            else
            {   logger_.debug("Overloading key {}: {} <-- {}", key, baseTree.get<string>(key), treeData);
                newTree.put(key, treeData);
            }
        }
    }

  // Return the resulting tree.
    return newTree;
}

string ConfigReader::printTree (const propTree &pt, int level) const
{ // Print contents of property tree.
  // If on the first level, print the tree name.
    if (level==0)
    {   logger_.debug("");
        logger_.debug(" === Config: {} === ", fn_config_);
    }
  // If tree has data only, just print that.
    // if (pt.empty()) logger_.debug("\"{}\"", pt.data());
    if (pt.empty()) return string("\"") + pt.data() + "\"";
  // Otherwise print the rest of the tree's values.
    else // Print all
        for (ptreeIter pos = pt.begin(); pos != pt.end(); ++pos)
        {   if(pos->second.data().empty())
            {   logger_.debug("{}\"{}\":", string(2*(level+1), ' '), pos->first);
                printTree(pos->second, level+1);
            }
            else logger_.debug("{}\"{}\": {}", string(2*(level+1), ' '), pos->first, printTree(pos->second, level+1));
        }
    if (level==0) logger_.debug("");
    return "";
}
