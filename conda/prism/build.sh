#!/bin/bash
mkdir -p $PREFIX/etc/conda/activate.d
mkdir -p $PREFIX/etc/conda/deactivate.d
cp $RECIPE_DIR/activate.d/env_vars.sh $PREFIX/etc/conda/activate.d/env_vars.sh
cp $RECIPE_DIR/deactivate.d/env_vars.sh $PREFIX/etc/conda/deactivate.d/env_vars.sh

cd $RECIPE_DIR/../../
make -f Makefile-as-dll -j 4
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include


mv libprism.so $PREFIX/lib
cp -r include/prism $PREFIX/include
