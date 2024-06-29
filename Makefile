WARNING_FLAGS = -Wall -Wextra -Wpedantic -Werror
CXXFLAGS = -std=c++17 -O3 $(WARNING_FLAGS)
# Source file and executable name
SRC = main.C
EXE = main

PROJECT = prism
# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Source and header files
SOURCES = $(wildcard $(SRCDIR)/*.C)
HEADERS = $(wildcard $(INCDIR)/*.h)
OBJECTS = $(patsubst $(SRCDIR)/%.C,$(BUILDDIR)/%.o,$(SOURCES))

# adding include to look in the conda environment
INCLUDE_PATH = -I$(CONDA_PREFIX)/include
# link instructions to look in the conda environment
LIBRARY_PATH = -L$(CONDA_PREFIX)/lib

YAML_LINK = -lyaml-cpp
FMT_LINK = -lfmt

LINKS = $(YAML_LINK) $(FMT_LINK)

# build both reaction parser and yaml library
all: $(EXE)

# build just the reaction parser
$(EXE): $(OBJECTS)
	@echo "Building $(EXE)"
	@$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(LIBRARY_PATH) -I$(INCDIR) $(OBJECTS) $(SRC) -o $(EXE) $(LINKS)
	@echo "$(EXE) built successfully!"

# build all of the source files for the parser
$(BUILDDIR)/%.o: $(SRCDIR)/%.C
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -I$(INCDIR)/$(PROJECT) $(INCLUDE_PATH) -c $< -o $@
	@echo "Building $<"
# clean up the parser
clean:
	@rm -rf *.dSYM
	@rm -f $(EXE)
	@rm  -rf build
