#!/bin/bash
cd $RECIPE_DIR/../../
make -f Makefile-as-dll -j 4
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include


mv libprism.so $PREFIX/lib
cp -r include/prism $PREFIX/include
