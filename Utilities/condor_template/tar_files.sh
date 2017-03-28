#!/bin/bash

# Give input config files generic names
cp $3 dsList.txt

# Compress files
tar czv --file=zcfiles.tgz --files-from=files_to_tar.txt

# Clean up previous files.
rm dsList.txt
