# [ZCAnalyzer][ZCAnalyzer github]

A number of programs used for the analysis of data collected by the [Compact Muon Solenoid experiment][CMS] at the [LHC][LHC]. These analyses are used to study the cross section of a Z boson created in association with a c-quark jet.

These programs run on [ROOT][ROOT] ntuples created from CMS MINIAOD data by an [ntupler][Ntupler] adapted from [CMS VHbb group][VHbb].

Project created from template by [Hiltmon][Simple C++ Project Structure].

## Subprojects

* [NtupleProcessor][NtupleProcessor github]

## Installation Instructions

Fetch submodules:

    git submodule init
    git submodule update

Upgrading all bundled plugins: 

    git submodule foreach git pull origin master

Make all software from head directory.

    make

### Requirements

Compilation requires:

* [ROOT 6][ROOT]
* [Boost C++ Libraries][Boost]

## Data Locations

## Links

* [Buffalo Z+c Run I Analysis TWiki][Z+c RunI Twiki]

## Author

Created by Andrew Godshalk  
Graduate Student, SUNY at Buffalo  
[email][Andrew email] - [website][Andrew website] - [github][Andrew github]

[//]: # (Reference Links)

[LHC]:https://home.cern/topics/large-hadron-collider
[CMS]:https://home.cern/about/experiments/cms
[ROOT]:https://root.cern.ch/
[Boost]:http://www.boost.org/
[VHbb]:https://twiki.cern.ch/twiki/bin/view/CMS/HiggsBB
[Ntupler]: https://github.com/andrewgodshalk/ZCNtupler
[Simple C++ Project Structure]: http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
[Andrew github]:https://github.com/andrewgodshalk/
[Andrew email]:andrewgodshalk@gmail.com
[Andrew website]:http://www.acsu.buffalo.edu/~godshalk/
[Z+c RunI Twiki]:https://twiki.cern.ch/twiki/bin/viewauth/CMS/ZplusHF8TeV
[ZCAnalyzer github]:https://github.com/andrewgodshalk/ZCAnalyzer
[NtupleProcessor github]:https://github.com/andrewgodshalk/ZCRunII/tree/master/NtupleProcessor
