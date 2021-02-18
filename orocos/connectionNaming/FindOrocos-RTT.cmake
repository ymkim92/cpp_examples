
# Lookup the Orocos::RTT compile and link flags.

# Sets the following variables (example: OROCOS_TARGET = lxrt)
#
# OS_LXRT                (defined)      
# OROCOS-RTT_FOUND       (defined)
# OROCOS-RTT_INCLUDE_DIRS    Absolute path to the include directory
# OROCOS-RTT_LIBRARIES       Absolute path to the orocos-rtt library and all requested components
# OROCOS-RTT_DEFINITIONS The -DOROCOS_TARGET=lxrt macro
# OROCOS-RTT_VERSION     For the detected API version 
#
# For each component listed in find_package the variable OROCOS-RTT_<COMPONENT>_LIBRARIES is defined as well.
# The component name is lower case (the same case as the library name), the variable name is upper case.
#
# Example Usage: find_package(Orocos-RTT 1.6 REQUIRED corba)
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

message( STATUS "Looking for RTT version v${Orocos-RTT_FIND_VERSION}" )
set(OROCOS_TARGET "gnulinux" CACHE STRING "The (RT-)OS target we are compiling for. One of: gnulinux, macosx, lxrt, xenomai.")
set(OROCOS_CORBA_TARGET "corba-${OROCOS_TARGET}" )
string(TOUPPER ${OROCOS_TARGET} OROCOS_TARGET_CAPS )

find_path(OROCOS-RTT_INCLUDE_DIR rtt/os/targets/${OROCOS_TARGET} )
find_path(OROCOS-RTT_INCLUDE_DIR rtt/os/${OROCOS_TARGET}/${OROCOS_TARGET}.h )

# Extract version information.
IF(OROCOS-RTT_INCLUDE_DIR)
  # Extract OROCOS-RTT_VERSION and OROCOS-RTT_LIB_VERSION from rtt-config.h
  # Read the whole file:
  #
  FILE(READ "${OROCOS-RTT_INCLUDE_DIR}/rtt/rtt-config.h" _OROCOS-RTT_RTT_CONFIG_HPP_CONTENTS)
  
  STRING(REGEX REPLACE ".*#define RTT_VERSION [ ]*([0-9]+.[0-9]+.[0-9]+).*" "\\1" OROCOS-RTT_VERSION "${_OROCOS-RTT_RTT_CONFIG_HPP_CONTENTS}")
  STRING(REGEX REPLACE ".*#define RTT_VERSION_MAJOR ([0-9]+).*" "\\1" OROCOS-RTT_VERSION_MAJOR "${_OROCOS-RTT_RTT_CONFIG_HPP_CONTENTS}")
  STRING(REGEX REPLACE ".*#define RTT_VERSION_MINOR ([0-9]+).*" "\\1" OROCOS-RTT_VERSION_MINOR "${_OROCOS-RTT_RTT_CONFIG_HPP_CONTENTS}")
  STRING(REGEX REPLACE ".*#define RTT_VERSION_PATCH ([0-9]+).*" "\\1" OROCOS-RTT_VERSION_PATCH "${_OROCOS-RTT_RTT_CONFIG_HPP_CONTENTS}")

  SET(Orocos-RTT_VERSION ${OROCOS-RTT_VERSION})
  enforce_version(Orocos-RTT "${OROCOS-RTT_INCLUDE_DIR}/rtt/rtt-config.h")

  #MESSAGE( STATUS "Found       RTT version v${OROCOS-RTT_VERSION}" )
ELSE(OROCOS-RTT_INCLUDE_DIR)
  MESSAGE(SEND_ERROR "Unable to locate RTT include files.")
ENDIF(OROCOS-RTT_INCLUDE_DIR)

#  Check version number 
#  user's "1" is passed as "1.0.0"
#  user's "1.4" is passed as "1.4.0"
if ( ${OROCOS-RTT_VERSION} LESS ${Orocos-RTT_FIND_VERSION})
  MESSAGE(SEND_ERROR "rtt-config.h reports version ${OROCOS-RTT_VERSION} < required version ${Orocos-RTT_FIND_VERSION}")
endif()

find_library(OROCOS-RTT_LIBRARY orocos-rtt-${OROCOS_TARGET} )

set(OROCOS-RTT_INCLUDE_DIRS ${OROCOS-RTT_INCLUDE_DIR} )
set(OROCOS-RTT_LIBRARIES ${OROCOS-RTT_LIBRARY})
set(OROCOS-RTT_PROCESS_LIBS OROCOS-RTT_LIBRARY)

foreach (COMPONENT ${Orocos-RTT_FIND_COMPONENTS})
    string(TOUPPER ${COMPONENT} UCOMPONENT)
    # Find an RTT shared library or plugin
    find_library (OROCOS-RTT_${UCOMPONENT}_LIBRARY NAMES "orocos-rtt-${COMPONENT}-${OROCOS_TARGET}")
    list (APPEND OROCOS-RTT_PROCESS_LIBS OROCOS-RTT_${UCOMPONENT}_LIBRARY)
    if (OROCOS-RTT_${UCOMPONENT}_LIBRARY)
      set (OROCOS-RTT_${UCOMPONENT}_FOUND TRUE)
      list (APPEND OROCOS-RTT_FOUND_COMPONENTS ${COMPONENT})
    else ()
      set (OROCOS-RTT_${UCOMPONENT}_FOUND FALSE)
      set (OROCOS-RTT_FOUND FALSE)
      list (APPEND OROCOS-RTT_MISSING_COMPONENTS ${COMPONENT})
    endif ()
    mark_as_advanced (OROCOS-RTT_${UCOMPONENT}_LIBRARY)
endforeach ()

if (NOT OROCOS-RTT_MISSING_COMPONENTS)
  message("Found these RTT Components: ${OROCOS-RTT_FOUND_COMPONENTS}")
else()
  message("These RTT Components were missing: ${OROCOS-RTT_MISSING_COMPONENTS}")
endif()


find_library(OROCOS-RTT_CORBA_LIBRARY orocos-rtt-${OROCOS_CORBA_TARGET} )

# Final check
find_package_handle_standard_args(Orocos-RTT DEFAULT_MSG ${OROCOS-RTT_PROCESS_LIBS} OROCOS-RTT_INCLUDE_DIR)

# Finally set the user variables.
if (OROCOS-RTT_FOUND)
  set(OS_${OROCOS_TARGET_CAPS} 1)
  set(OROCOS-RTT_DEFINITIONS "OROCOS_TARGET=${OROCOS_TARGET}")
  set(OROCOS-RTT_LIBRARIES ${OROCOS-RTT_LIBRARY})
  set(OROCOS-RTT_INCLUDE_DIRS ${OROCOS-RTT_INCLUDE_DIR})
  
  foreach( COMPONENT ${Orocos-RTT_FIND_COMPONENTS} )

    string(TOUPPER ${COMPONENT} UCOMPONENT)
    mark_as_advanced( OROCOS-RTT_${UCOMPONENT}_LIBRARY )
    #note: is this standard practice ? To add each component library to the main libraries variable.
    list (APPEND OROCOS-RTT_LIBRARIES ${OROCOS-RTT_${UCOMPONENT}_LIBRARY})
    set( OROCOS-RTT_${UCOMPONENT}_LIBRARIES ${OROCOS-RTT_${UCOMPONENT}_LIBRARY} )

  endforeach()
endif()
