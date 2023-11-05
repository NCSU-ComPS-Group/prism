# This script uninstalls files from an installation.

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  message(FATAL_ERROR "Installation directory not provided. Aborting uninstall.")
endif()

file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")
