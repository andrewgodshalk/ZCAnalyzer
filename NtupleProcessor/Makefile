# Makefile for ZCRunII/NtupleProcessor executable
# Last modified 2016-10-14
# Copied from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
# TO DO:
# - Figure out a good way to add dependancies to utility classes.
# - Figure out best way to include header files in dependancies.

#CC := g++
CC := g++-4.9
SRCDIR := src
HDRDIR := include
UTILDIR := ../Utilities/include
SUBMODDIR := ../Utilities/submodules
BUILDDIR := build
TARGETDIR := bin
TARGET := bin/NtupleProcessor

SRCEXT := cpp
HDREXT := h
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
ROOTFLAGS=`root-config --cflags --glibs`
BOOSTLIB := -lboost_program_options
CFLAGS := -g -Wall -std=c++11 $(BOOSTLIB) $(ROOTFLAGS)
INC := -I $(HDRDIR) -I $(UTILDIR) -I $(SUBMODDIR)/spdlog/include/

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
regexSPTest:
	$(CC) $(CFLAGS) spike/regexSPTest/main.cpp $(INC) $(LIB) -o bin/regexSPTest

.PHONY: clean
