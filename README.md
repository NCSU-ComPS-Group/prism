# Required Tools

## Mamba

Use of a package manager is recommended for use of this program. This will help avoid potentialy library conflicts and ensure build instructions work. A robust manager is mamba this can be installed from [https://mooseframework.inl.gov/getting_started/installation/conda.html](https://mooseframework.inl.gov/getting_started/installation/conda.html)

When installing mamba using these instructions you can stop after the instruction to restart you terminal is given

## Creating a new environment

To ensure no conflicts with other projects you can create a new environment to work in.

```bash
  conda create --name rxn_mech
  conda activate rxn_mech
  conda install -c conda-forge cmake
  conda install -c conda-forge fmt
  conda install -c conda-forge yaml-cpp
  conda install -c conda-forge gtest
```

# Installation

```bash
  conda activate rxm_mech
  mkdir -p ~/projects
  cd ~/projects
  git clone https://github.com/gsgall/reaction_parser.git
  cd reaction_parser
```
```bash
  git submodule update --init yaml-cpp
```

# Build
## Parser and yaml-cpp

* Please note that when buildling yaml-cpp several warnings may appear, you can ignore these.

```bash
  make all
```

## Parser Only

```bash
  make
```

## yaml-cpp Only

```bash 
  make build_yaml
```

## Clean Up

This will delete all of the files created when building the parser.

```bash
  make clean
```

This will delete all of the files created when building the parser and yaml-cpp.

```bash
  make clobberall
```

# Running Unit Tests

```bash
  cd test
  make
  ./run_tests
```
# Remove Environment

If you'd like to remove the environment for any reason you can use the following

```bash
  mamba activate base
  mamba env remove -n rxn_mech
```

# Trouble Shooting

If you are getting the error when running main where it is looking for the library in a different location you may need to update your search path. You can modify your conda environment variables. First you need create some files and folders

```bash
  cd $CONDA_PREFIX
  mkdir -p ./etc/conda/activate.d
  mkdir -p ./etc/conda/deactivate.d
  touch ./etc/conda/activate.d/env_vars.sh
  echo 'export DYLD_LIBRARY_PATH="$CONDA_PREFIX/lib:$DYLD_LIBRARY_PATH"' > ./etc/conda/activate.d/env_vars.sh
  touch ./etc/conda/deactivate.d/env_vars.sh
  echo 'unset DYLD_LIBRARY_PATH' > ./etc/conda/deactivate.d/env_vars.sh
```

The following commands are the same but for a linux system.

```bash
  cd $CONDA_PREFIX
  mkdir -p ./etc/conda/activate.d
  mkdir -p ./etc/conda/deactivate.d
  touch ./etc/conda/activate.d/env_vars.sh
  echo 'export LD_LIBRARY_PATH="$CONDA_PREFIX/lib:$LD_LIBRARY_PATH"' > ./etc/conda/activate.d/env_vars.sh
  touch ./etc/conda/deactivate.d/env_vars.sh
  echo 'unset LD_LIBRARY_PATH' > ./etc/conda/deactivate.d/env_vars.sh
```
