# Installation

```bash
  conda activate PRISM
  mkdir -p ~/projects
  cd ~/projects
  git clone https://github.com/gsgall/PRISM
  cd ~/projects/PRISM
```

# Mamba

Use of a package manager is recommended for use of this program. This will help avoid potentialy library conflicts and ensure build instructions work. A robust manager is mamba this can be installed from [https://mooseframework.inl.gov/getting_started/installation/conda.html](https://mooseframework.inl.gov/getting_started/installation/conda.html)

When installing mamba using these instructions you can stop after the instruction to restart you terminal is given

# Creating a new environment

To ensure no conflicts with other projects you can create a new environment to work in.

```bash
  cd ~/projects/PRISM
  conda create --name PRISM
  conda activate PRISM
  conda install --file environment.yml
```

# Build


```bash
  make -j 4
```

# Build The Docs

Since this system relies on the MOOSE documentation system you will need MOOSE installed to build and host the documentation site locally. We expect you to have MOOSE installed  at `~/projects/moose` if you would like to do this.

```bash
conda activate moose
cd ~/projects/PRISM/doc
MOOSE_DIR=~/projects/moose ROOT_DIR=./ ./moosedocs.py build --serve
```

# Running Unit Tests

```bash
  cd test
  make
  ./run_tests -j 4
```

# Remove Environment

If you'd like to remove the environment for any reason you can use the following

```bash
  conda activate base
  conda env remove -n PRISM
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
  cd -
  conda deactivate
  conda activate PRISM
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
  cd -
  conda deactivate
  conda activate PRISM
```
