# Makefile for ZCRunII
# Compiles all programs directly in subfolders.
# Modified: 2016-10-24
# TO DO: ----

LOGDIR := logs
HISTDIR := output

all: NtupleProcessor
	@mkdir -p $(LOGDIR)
	@mkdir -p $(HISTDIR)
	make -C NtupleProcessor

clean:
	make clean -C NtupleProcessor
