if [[ $(uname) == Darwin ]]; then
  unset DYLD_LIBRARY_PATH
else
  unset LD_LIBRARY_PATH
fi

unset CXX
