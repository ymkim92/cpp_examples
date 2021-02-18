cmake_minimum_required(VERSION 2.6)
include(CreateComponent.cmake)

# WORKAROUND for bug 684 with boost installation from MacPorts.
INCLUDE_DIRECTORIES(/opt/local/include)

# create shared library ready for deployment
create_component(ConnectionNaming VERSION 1.0.0 
  HMI.cpp 
  Robot.cpp
  OneAxisFilter.cpp)

# for Mac OS X, you have to link against all dependant libraries
TARGET_LINK_LIBRARIES(ConnectionNaming
  ${RTT_LIBS})
