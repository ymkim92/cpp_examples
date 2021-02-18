
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/")

#
# - Create an Orocos component from a given cpp file.
# Usage:
#  create_component(<component-name> [VERSION <version>] <files> )
#
# Example:
#  create_component(quicky VERSION 1.4 quicky.cpp)
#
# Creates the quicky library with version 1.4 (if supported by your OS) from the file quicky.cpp
#
# The VERSION argument is the version of your component, not the RTT version.
# This macro defaults to creating components with RTT/OCL 1.6.0 or later.
# If you want another version of OrocosRTT, call find_package(OrocosRTT <my.rtt.version> REQUIRED)
# before you call this macro.
#

macro(create_component COMPONENT_NAME )

  find_package(Orocos-RTT 1.6.0 REQUIRED)
  find_package(Orocos-OCL 1.6.0 REQUIRED)

  message( "Building Shared library for ${COMPONENT_NAME}" )

  foreach( arg ${ARGN} )
    if ( ${arg} STREQUAL VERSION )
      SET(IN_VERSION 1)   
    else()
      if(IN_VERSION)
	#Process version argument and store result for use in set_target_properties
	set(_VERSION ${arg})
	string( REGEX MATCHALL "[0-9]+" _VERSIONS ${_VERSION} )
	list( GET _VERSIONS 0 _VERSION_MAJOR)
	list( GET _VERSIONS 1 _VERSION_MINOR)
	list( GET _VERSIONS 2 _VERSION_PATCH)
	list(LENGTH _VERSIONS vlength)
	if ( ${vlength} EQUAL 1 )
	  set( VERSION_SPEC VERSION ${_VERSION} SOVERSION ${_VERSION_MAJOR} )
	elseif(${vlength} EQUAL 2)
	  set( VERSION_SPEC VERSION ${_VERSION} SOVERSION ${_VERSION_MAJOR} )
	elseif(${vlength} EQUAL 3)
	  set( VERSION_SPEC VERSION ${_VERSION} SOVERSION ${_VERSION_MAJOR}.${_VERSION_MINOR} )
	endif()

	set(IN_VERSION 0)
      else()
	list(APPEND _files ${arg})
      endif()
    endif()
  endforeach()

  add_library( ${COMPONENT_NAME} SHARED ${_files} )
  include_directories(${OROCOS-RTT_INCLUDE_DIRS})
  set_target_properties( ${COMPONENT_NAME} PROPERTIES 
    DEFINE_SYMBOL OCL_DLL_EXPORT 
    COMPILE_DEFINITIONS "${OROCOS-RTT_DEFINITIONS}"
    ${VERSION_SPEC}
    )
  target_link_libraries( ${COMPONENT_NAME} ${OROCOS-RTT_LIBRARIES} ${OROCOS-OCL_LIBRARIES} )

endmacro()