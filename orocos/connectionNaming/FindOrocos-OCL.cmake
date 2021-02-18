
# Lookup the Orocos::OCL compile and link flags.

# Sets the following variables (example: OROCOS_TARGET = lxrt)
#
# OS_LXRT                (defined)      
# OROCOS-OCL_FOUND       (defined)
# OROCOS-OCL_INCLUDE_DIRS    Absolute path to the include directory
# OROCOS-OCL_LIBRARIES       Absolute path to the selected library components
# OROCOS-OCL_VERSION     For the detected API version 
#
# For each component listed in find_package the variable OROCOS-OCL_<COMPONENT>_LIBRARIES, in case of a library,
# and OROCOS-OCL_<COMPONENT>_PROGRAM, in case of a program, is defined as well.
# The component/program name is lower case (the same case as the library/program name), the variable name is upper case.
#
# Example Usage: find_package(Orocos-OCL 1.6 COMPONENTS deployer ctaskbrowser reporting)
#
# See the cmake find_package help for all possible options.

include(FindPackageHandleStandardArgs)

################################################################################
#
# Enforces the package version in a find_package command by means of the
# optional VERSION and EXACT parameters. If no version is specified, this
# function does nothing.
#
# The following parameters are REQUIRED:
#
# - package_NAME: String containing the name of the package
# - _source: String containing the name source of the detected version (a .h file or library or...)
#
# This function assumes that a variable named ${package_NAME}_VERSION exists.
################################################################################

function(enforce_version package_NAME _source)

  if(${package_NAME}_FIND_VERSION_COUNT GREATER 0)
    # Exact version enforced
    if(${package_NAME}_FIND_VERSION_EXACT)
      if(NOT ${package_NAME}_VERSION VERSION_EQUAL ${package_NAME}_FIND_VERSION)
        message(FATAL_ERROR "${package_NAME} version not an EXACT match. Detected version: ${${package_NAME}_VERSION} (from: ${_source}), requested version: ${${package_NAME}_FIND_VERSION}.")
      endif(NOT ${package_NAME}_VERSION VERSION_EQUAL ${package_NAME}_FIND_VERSION)
    # Exact version NOT enforced
    else(${package_NAME}_FIND_VERSION_EXACT)
      if(${package_NAME}_VERSION VERSION_LESS ${package_NAME}_FIND_VERSION)
        message(FATAL_ERROR "${package_NAME} version too old. Detected version: ${${package_NAME}_VERSION} (from: ${_source}), requested version: ${${package_NAME}_FIND_VERSION}.")
      endif(${package_NAME}_VERSION VERSION_LESS ${package_NAME}_FIND_VERSION)
    endif(${package_NAME}_FIND_VERSION_EXACT)
  endif(${package_NAME}_FIND_VERSION_COUNT GREATER 0)

endfunction(enforce_version)

message( STATUS "Looking for OCL version v${Orocos-OCL_FIND_VERSION}" )
set(OROCOS_TARGET "gnulinux" CACHE STRING "The (RT-)OS target we are compiling for. One of: gnulinux, macosx, lxrt, xenomai.")
set(OROCOS_CORBA_TARGET "corba-${OROCOS_TARGET}" )

find_path(OROCOS-OCL_INCLUDE_DIR ocl/ocl-config.h )

# Extract version information.
if(OROCOS-OCL_INCLUDE_DIR)
  # Extract OROCOS-OCL_VERSION and OROCOS-OCL_LIB_VERSION from ocl-config.h
  # Read the whole file:
  #
  file(READ "${OROCOS-OCL_INCLUDE_DIR}/ocl/ocl-config.h" _OROCOS-OCL_OCL_CONFIG_HPP_CONTENTS)
  
  string(REGEX REPLACE ".*#define OCL_VERSION [ ]*([0-9]+.[0-9]+.[0-9]+).*" "\\1" OROCOS-OCL_VERSION "${_OROCOS-OCL_OCL_CONFIG_HPP_CONTENTS}")
  string(REGEX REPLACE ".*#define OCL_VERSION_MAJOR ([0-9]+).*" "\\1" OROCOS-OCL_VERSION_MAJOR "${_OROCOS-OCL_OCL_CONFIG_HPP_CONTENTS}")
  string(REGEX REPLACE ".*#define OCL_VERSION_MINOR ([0-9]+).*" "\\1" OROCOS-OCL_VERSION_MINOR "${_OROCOS-OCL_OCL_CONFIG_HPP_CONTENTS}")
  string(REGEX REPLACE ".*#define OCL_VERSION_PATCH ([0-9]+).*" "\\1" OROCOS-OCL_VERSION_PATCH "${_OROCOS-OCL_OCL_CONFIG_HPP_CONTENTS}")

  #message( STATUS "Found       OCL version v${OROCOS-OCL_VERSION}" )

  #  Check version number 
  #  user's "1" is passed as "1.0.0"
  #  user's "1.4" is passed as "1.4.0"
  set(Orocos-OCL_VERSION ${OROCOS-OCL_VERSION})
  enforce_version(Orocos-OCL "${OROCOS-OCL_INCLUDE_DIR}/ocl/ocl-config.h")
  
else()
  message( "OCL ocl-config.h file not found. Could not determine installed version. Assuming 1.4.0." )
  set(OROCOS-OCL_VERSION "1.4.0")
endif()

#Older OCL versions do not have ocl-config.h
find_path(OROCOS-OCL_INCLUDE_DIR ocl/OCL.hpp )

find_library(OROCOS-OCL_LIBRARY orocos-ocl-common-${OROCOS_TARGET} )

set( OROCOS-OCL_PROCESS_PROGRAMS )
set( OROCOS-OCL_PROCESS_LIBS OROCOS-OCL_LIBRARY)

foreach (COMPONENT ${Orocos-OCL_FIND_COMPONENTS})
  string(TOUPPER ${COMPONENT} UCOMPONENT)
  if (${COMPONENT} MATCHES ".*deployer.*" OR ${COMPONENT} MATCHES ".*taskbrowser.*")
    # special case for the deployer programs
    find_program (DEPLOYER_${UCOMPONENT}_EXECUTABLE "${COMPONENT}-${OROCOS_TARGET}" )
    list(APPEND OROCOS-OCL_PROCESS_PROGRAMS DEPLOYER_${UCOMPONENT}_EXECUTABLE)
    if (DEPLOYER_${UCOMPONENT}_EXECUTABLE)
      set (DEPLOYER_${UCOMPONENT}_FOUND TRUE)
      list (APPEND OROCOS-OCL_FOUND_COMPONENTS "${COMPONENT}")
    else ()
      set (DEPLOYER_${UCOMPONENT}_FOUND FALSE)
      set (OROCOS-OCL_FOUND FALSE)
      list (APPEND OROCOS-OCL_MISSING_COMPONENTS "${COMPONENT}")
    endif ()
    mark_as_advanced (DEPLOYER_${UCOMPONENT}_EXECUTABLE)
  else ()
    # Find an OCL shared library
    find_library (OROCOS-OCL_${UCOMPONENT}_LIBRARY NAMES "orocos-${COMPONENT}-${OROCOS_TARGET}")
    list (APPEND OROCOS-OCL_PROCESS_LIBS OROCOS-OCL_${UCOMPONENT}_LIBRARY)
    if (OROCOS-OCL_${UCOMPONENT}_LIBRARY)
      set (OROCOS-OCL_${UCOMPONENT}_FOUND TRUE)
      list (APPEND OROCOS-OCL_FOUND_COMPONENTS ${COMPONENT})
    else ()
      set (OROCOS-OCL_${UCOMPONENT}_FOUND FALSE)
      set (OROCOS-OCL_FOUND FALSE)
      list (APPEND OROCOS-OCL_MISSING_COMPONENTS ${COMPONENT})
    endif ()
    mark_as_advanced (OROCOS-OCL_${UCOMPONENT}_LIBRARY)
  endif ()
endforeach ()

if (NOT OROCOS-OCL_MISSING_COMPONENTS)
  message("Found these OCL Components: ${OROCOS-OCL_FOUND_COMPONENTS}")
else()
  message("These OCL Components were missing: ${OROCOS-OCL_MISSING_COMPONENTS}")
endif()

find_package_handle_standard_args(Orocos-OCL DEFAULT_MSG ${OROCOS-OCL_PROCESS_LIBS} ${OROCOS-OCL_PROCESS_PROGRAMS} OROCOS-OCL_INCLUDE_DIR )

if (OROCOS-OCL_FOUND)
  #Only mark as advanced when actually found. When not found, the user can see this and edit if necessary.
  mark_as_advanced(OROCOS-OCL_INCLUDE_DIR)
  mark_as_advanced(OROCOS-OCL_LIBRARY)

  set(OROCOS-OCL_INCLUDE_DIRS ${OROCOS-OCL_INCLUDE_DIR} )
  set(OROCOS-OCL_LIBRARIES ${OROCOS-OCL_LIBRARY})

  foreach( COMPONENT ${Orocos-OCL_FIND_COMPONENTS} )
    string(TOUPPER ${COMPONENT} UCOMPONENT)

    mark_as_advanced( OROCOS-OCL_${UCOMPONENT}_LIBRARY )
    list (APPEND OROCOS-OCL_LIBRARIES ${OROCOS-OCL_${UCOMPONENT}_LIBRARY})
    set( OROCOS-OCL_${UCOMPONENT}_LIBRARIES ${OROCOS-OCL_${UCOMPONENT}_LIBRARY} )

  endforeach()

endif()


