# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall


PROJECT_NAME = rxn-cpp
PROJECT_SRC_DIR = ../src
PROJECT_INCLUDE_DIR = ../include/$(PROJECT_NAME)
# Directories
TEST_SRC_DIR = tests
TEST_BUILD_DIR = build
TEST_EXECUTABLE = run_tests

# Find all of the projects sources
PROJECT_SOURCES := $(wildcard $(PROJECT_SRC_DIR)/*.C)
PROJECT_OBJECTS := $(patsubst $(PROJECT_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(PROJECT_SOURCES))
# Find all test source files in the test directory
TEST_SOURCES := $(wildcard $(TEST_SRC_DIR)/*.C)
TEST_OBJECTS := $(patsubst $(TEST_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(TEST_SOURCES))

# Include Google Test and Google Mock libraries
GTEST_DIR = $(CONDA_PREFIX)
GTEST_LIB_DIR = $(GTEST_DIR)/lib
GTEST_INCLUDE_DIR = $(GTEST_DIR)/include
GTEST_LIBS = -lgtest -lgtest_main
YAML_LINK = -lyaml-cpp
FMT_LINK = -lfmt

LINKS = $(YAML_LINK) $(FMT_LINK) $(GTEST_LIBS)

# Build rule for the project files
$(PROJECT_BUILD_DIR)/%.o: $(PROJECT_SRC_DIR)/%.C
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -I$(PROJECT_INCLUDE_DIR) -I$(GTEST_INCLUDE_DIR) -c $< -o $@

# Build rule for project objects
$(TEST_BUILD_DIR)/%.o: $(PROJECT_SRC_DIR)/%.C
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -I$(PROJECT_INCLUDE_DIR) -I$(GTEST_INCLUDE_DIR) -c $< -o $@

# Build rule for test objects
$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.C
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR) -c $< -o $@

# Build rule for test objects
$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.C
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR) -c $< -o $@

# Build rule for the test runner
$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(PROJECT_OBJECTS)
	@$(CXX) $(CXXFLAGS) -I../include  -I$(GTEST_INCLUDE_DIR) -L$(GTEST_LIB_DIR) -o $@ $^ $(LINKS)

.PHONY: all clean

all: $(TEST_EXECUTABLE)

clean:
	@rm -rf $(TEST_BUILD_DIR)/*.o
	@rm -f $(TEST_EXECUTABLE)
