# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall

# Directories
SRC_DIR = tests
BUILD_DIR = build
TEST_EXECUTABLE = run_tests

# Find all test source files in the test directory
TEST_SOURCES := $(wildcard $(SRC_DIR)/*.C)
TEST_OBJECTS := $(patsubst $(SRC_DIR)/%.C, $(BUILD_DIR)/%.o, $(TEST_SOURCES))

# Include Google Test and Google Mock libraries
GTEST_DIR = $(CONDA_PREFIX)
GTEST_LIB_DIR = $(GTEST_DIR)/lib
GTEST_INCLUDE_DIR = $(GTEST_DIR)/include
GTEST_LIBS = -lgtest -lgtest_main
RXN_LINK = -lrxn-cpp
YAML_LINK = -lyaml-cpp
FMT_LINK = -lfmt

LINKS = $(RXN_LINK) $(YAML_LINK) $(FMT_LINK) $(GTEST_LIBS)


# Build rule for test objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.C
	@mkdir -p build
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE_DIR) -c $< -o $@

# Build rule for the test runner
$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	@$(CXX) $(CXXFLAGS) -I$(GTEST_INCLUDE_DIR) -L$(GTEST_LIB_DIR) -o $@ $^ $(LINKS)

.PHONY: all clean

all: $(TEST_EXECUTABLE)

clean:
	@rm -rf $(BUILD_DIR)/*.o
	@rm -f $(TEST_EXECUTABLE)
