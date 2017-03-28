#!/bin/bash

# Set up env and unpack all files
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH="slc6_amd64_gcc530"
cd ${_CONDOR_SCRATCH_DIR}
pwd
cd /uscms_data/d2/godshalk/root6SetupDir/CMSSW_8_0_25/src
eval `scramv1 runtime -sh`
cd -

tar -zxf zcfiles.tgz
mkdir -p logs/condor/
mkdir output/

# Set up variables to select files to run cmsRun, ZCAnalyzer over
let jobNum=$1+1
dsInfo=`sed "${jobNum}q;d" dsList.txt`
echo "  Dataset Info: $dsInfo"
read ntuple <<< $dsInfo
echo $ntuple

# Run the main process
NtupleProcessor/bin/NtupleProcessor -n $ntuple -q -b

# Rename the log file.
mv logs/NtupleProcessor_*.log logs/$ntuple.log

# Clean up
mv output/* .
mv logs/*.log .
rm -r NtupleProcessor
rm ZCLibrary/ -r
rm dsList.txt
rm zcfiles.tgz
