# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -Werror
# Source file and executable name
SRC = main.C
EXE = main

# Directories
SRCDIR = src
INCDIR = include

# adding include to look in the conda environment
INCLUDE_PATH = -I$(CONDA_PREFIX)/include
# link instructions to look in the conda environment
RXN_LINK = -lrxn-cpp
YAML_LINK = -lyaml-cpp
FMT_LINK = -lfmt

LINKS = $(RXN_LINK) $(YAML_LINK) $(FMT_LINK)
# build just the reaction parser
$(EXE):
	@$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -L$(CONDA_PREFIX)/lib $(SRC) -o $(EXE) $(LINKS)

# clean up the parser
clean:
	@rm -rf *.dSYM
	@rm -f $(EXE)
