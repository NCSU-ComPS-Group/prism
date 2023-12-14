# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -fprofile-arcs -ftest-coverage

PROJECT_NAME = rxn-cpp
PROJECT_SRC_DIR = ../src
PROJECT_INCLUDE_DIR = ../include/$(PROJECT_NAME)

TEST_SRC_DIR = tests
TEST_BUILD_DIR = build
TEST_EXECUTABLE = run_tests

CONDA_LIB_DIR = $(CONDA_PREFIX)/lib
CONDA_INCLUDE_DIR = $(CONDA_PREFIX)/include

GTEST_LIBS = -lgtest -lgtest_main
YAML_LINK = -lyaml-cpp
FMT_LINK = -lfmt

LIBRARY_LINKS =  $(YAML_LINK) $(FMT_LINK)
TEST_LINKS = $(YAML_LINK) $(FMT_LINK) $(GTEST_LIBS)


PROJECT_SOURCES := $(wildcard $(PROJECT_SRC_DIR)/*.C)
PROJECT_OBJECTS := $(patsubst $(PROJECT_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(PROJECT_SOURCES))

TEST_SOURCES := $(wildcard $(TEST_SRC_DIR)/*.C)
TEST_OBJECTS := $(patsubst $(TEST_SRC_DIR)/%.C, $(TEST_BUILD_DIR)/%.o, $(TEST_SOURCES))

$(TEST_BUILD_DIR)/%.o: $(PROJECT_SRC_DIR)/%.C
	@mkdir -p build
	@echo $@
	@$(CXX) $(CXXFLAGS) -I$(CONDA_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.C
	@mkdir -p build
	@echo $@
	@$(CXX) $(CXXFLAGS) -I../include -I$(CONDA_INCLUDE_DIR) -c $< -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(PROJECT_OBJECTS)
	@$(CXX) $(CXXFLAGS) -I../include -I$(CONDA_INCLUDE_DIR) -L$(CONDA_LIB_DIR) -o $@ $^ $(TEST_LINKS)

.PHONY: all clean coverage

all: $(TEST_EXECUTABLE)

clean:
	@rm -rf $(TEST_BUILD_DIR)/*.o
	@rm -f $(TEST_EXECUTABLE)
	@rm -rf $(PROJECT_BUILD_DIR)/*.o
	@rm -rf coverage_report
	@rm -rf build/*.g*
	@rm -rf *.dSYM


coverage: all
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info "*/include/*" -o coverage.info
	lcov --remove coverage.info "*/gtest/*" -o coverage.info
	lcov --remove coverage.info "*/v1/*" -o coverage.info
	lcov --remove coverage.info "*/test/*" -o coverage.info
	lcov --remove coverage.info "*/yaml-cpp/*" -o coverage.info
	lcov --remove coverage.info "*/fmt/*" -o coverage.info
	genhtml coverage.info --output-directory coverage_report
	open coverage_report/index.html
