if [[ $(uname) == Darwin ]]; then
  export DYLD_LIBRARY_PATH="$CONDA_PREFIX/lib:$DYLD_LIBRARY_PATH"
  export CXX=$(which clang++)
else
  export LD_LIBRARY_PATH="$CONDA_PREFIX/lib:$LD_LIBRARY_PATH"
  export CXX=$(which g++)
fi
