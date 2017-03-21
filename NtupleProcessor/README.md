# [NtupleProcessor][NtupleProcessor github]

Executable used to create histograms from Z+c ntuples for various analysis purposes.

This program runs on [ROOT][ROOT] ntuples created from CMS MINIAOD data by an [ntupler][Ntupler] adapted from [CMS VHbb group][VHbb].

Project created from template by [Hiltmon][Simple C++ Project Structure].

This program is part of the [ZCRunII][ZCRunII github] project.

## Installation & Usage

```
git clone git@github.com:andrewgodshalk/ZCRunII.git
make
./NtupleProcessor/bin/NtupleProcessor.exe -OPTIONS
```

Requires an installation of [ROOT 6][ROOT] and [Boost C++ Libraries][Boost]. Makefile is configured to include libraries automatically.

## Data Locations

# Links

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
[Ntupler]:https://github.com/andrewgodshalk/cmssw/tree/ZC2015
[Simple C++ Project Structure]: http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
[Andrew github]:https://github.com/andrewgodshalk/
[Andrew email]:andrewgodshalk@gmail.com
[Andrew website]:http://www.acsu.buffalo.edu/~godshalk/
[Z+c RunI Twiki]:https://twiki.cern.ch/twiki/bin/viewauth/CMS/ZplusHF8TeV
[ZCRunII github]:https://github.com/andrewgodshalk/ZCRunII
[NtupleProcessor github]:https://github.com/andrewgodshalk/ZCRunII/tree/master/NtupleProcessor
