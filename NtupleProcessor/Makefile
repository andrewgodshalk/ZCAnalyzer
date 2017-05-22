# Makefile for ZCRunII/NtupleProcessor executable
# Last modified 2016-10-14
# Copied from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
# Relying on symbolic links to link source in ZCLibrary. Because I'm in a hurry.
#
# TO DO:
# - Learn how a GNU makefile actually works.
# - Figure out a good way to add dependancies to utility classes.
# - Figure out best way to include header files in dependancies.

#CC := g++
CC := g++-4.9
SRCDIR := src
HDRDIR := include
UTILDIR := ../Utilities
SUBMODDIR := $(UTILDIR)/submodules
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/NtupleProcessor

SRCEXT := cpp
HDREXT := h
# SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT)) $(shell find $(UTILDIR)/$(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
ROOTFLAGS :=`root-config --cflags --glibs`
BOOSTLIB := -lboost_program_options
CFLAGS := -g -Wall -std=c++11 -fdiagnostics-color $(BOOSTLIB) $(ROOTFLAGS)
INC := -I $(HDRDIR) -I $(UTILDIR)/$(HDRDIR) -I $(SUBMODDIR)/spdlog/include/
# INC := -I $(HDRDIR) -I $(SUBMODDIR)/spdlog/include/

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(BOOSTLIB) $(ROOTFLAGS)"; $(CC) $^ -o $(TARGET) $(BOOSTLIB) $(ROOTFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HDRDIR)/%.$(HDREXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(TARGETDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGETDIR)"; $(RM) -r $(BUILDDIR) $(TARGETDIR)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
# regexSPTest:
# 	$(CC) $(CFLAGS) spike/regexSPTest/main.cpp $(INC) $(LIB) -o bin/regexSPTest

.PHONY: clean
