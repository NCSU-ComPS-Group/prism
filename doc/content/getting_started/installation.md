# Installation

## Conda/Mamba

Use of a package manager is recommended for use of this program. This will help avoid potential library conflicts and ensure build instructions work. A robust manager is mamba this can be installed from the [MOOSE website](https://mooseframework.inl.gov/getting_started/installation/conda.html).

When installing mamba using these instructions you can stop after the instruction to restart your terminal is given.

# Conda Package

PRISM manages all of its dependencies via conda.
Currently these packages are not available on a conda channel but we are working on making this available.
Please see [issue #27](https://github.com/NCSU-ComPS-Group/prism/issues/27) for potential updates.

## Conda Environment

You may build and install the `prism-dev` conda package locally and instally it as if it were downloaded from a conda channel.
This package contains our build tool chain and all of the dependencies required to build and use PRISM.

```bash
cd ~/projects
git clone https://github.com/NCSU-ComPS-Group/prism.git
cd ~/projects/prism
conda install -y conda-build
mkdir -p build
conda build conda/prism-dev --output-folder build
conda create -y -n prism-dev
conda activate prism-dev
conda install -y prism-dev -c ./build
```

These instructions will create a new conda environment called `prism-dev` which will have everything you need to build PRISM.

## Library Installation

PRISM supports being built as a dynamically linkable library for use in other projects.
In order to build and install this locally you will need to have the `prism-dev` environment configured.

``` bash
conda activate prism-dev
cd ~/projects/prism
make -f Makefile-as-dll -j 4 install
```

This will compile the PRISM project as a dynamically linkable library and install it in your current conda environment as if it were downloaded from a conda channel.
There are plans to create a conda package to support using PRISM in this way.
Please see [issue #27](https://github.com/NCSU-ComPS-Group/prism/issues/27) for potential updates.

