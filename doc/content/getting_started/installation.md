# Installation

!alert note
This package has only been tested on macOS and Linux platforms. If you are interested in using this package on a Windows device we suggest installing the [WSL](https://learn.microsoft.com/en-us/windows/wsl/install). Please note that we have not tested using this package in this way.

## Conda/Mamba

Use of a package manager is recommended for use of this program. This will help avoid potentialy library conflicts and ensure build instructions work. A robust manager is mamba this can be installed from the [MOOSE website](https://mooseframework.inl.gov/getting_started/installation/conda.html).

When installing mamba using these instructions you can stop after the instruction to restart you terminal is given.

## Conda Package

There are several ways to download/install the PRISM parser for personal use. If you are only interested in using PRISM for input into your own modeling/analysis software the easiest way to make this happen is by installing it with the the available [conda package](https://anaconda.org/gsgall/prism). To install this you can use the following commands.

```bash
  conda config --add channels gsgall
  conda install prism
```

## Building Directly

If you do not want to use PRISM via the conda package we suggest that you add prism as a [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to the repository in which you are building your application.

```bash
  git submodule add https://github.com/NCSU-ComPS-Group/prism.git
  git submodule update --init prism
```

 We also support a conda package to faciliate development work on PRISM. This package installs all of the required compilers, tools, and packages PRISM depends on. To download this use the following commands in your conda environment

!alert warning
This package adds a script to your conda environment which appends to the path where dynamically linkable libraries are searched for (LD_LIBRARY_PATH for macOS or DYLD_LIBRARY_PATH for Linux devices). This package also sets the environment variable (CXX)

```bash
  conda config --add channels gsgall
  conda install prism-dev
```
