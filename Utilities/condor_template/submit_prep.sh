#!/bin/bash

# Give input config files generic names
cp $1 dsList.txt

# Compress files
tar czv --file=zcfiles.tgz --files-from=files_to_tar.txt

# Clean up previous files.
rm dsList.txt

# Create output directories
mkdir -p logs/condor/
mkdir output/
