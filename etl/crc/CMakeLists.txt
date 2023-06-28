cmake_minimum_required(VERSION 3.14.4)
project(crc_etl_test VERSION 1.0.0)
set(CMAKE_SYSTEM_NAME Linux C++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(C_STANDARD 11)
set(C_EXTENSIONS ON)

set(ETL_VERSION 20.35.14)
set(ETL_GIT_TAG 20.35.14)

find_package(PkgConfig REQUIRED)

# Build src only instead of full super build
if (DEFINED SRC_BUILD_ONLY)
    add_subdirectory(src)
    return()
endif()

set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/staged)
message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")

# Workaround for yocto build env setup which sets ENV{CXX} 
# and it doesn't get passed through ExternalProject_Add
# if (DEFINED CMAKE_CXX_COMPILER_ARG1)
#     set(CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_COMPILER_ARG1}")
# endif()

add_subdirectory(external/upstream)

include(ExternalProject)
ExternalProject_Add(crc_etl_test
    DEPENDS
        etl_external
    SOURCE_DIR
        ${CMAKE_CURRENT_LIST_DIR}/src
    CMAKE_ARGS
        -DENABLE_TESTS=${ENABLE_TESTS}
        -DCMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}
        -DCMAKE_CXX_CLANG_TIDY=${CMAKE_CXX_CLANG_TIDY}
        -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -Detl_DIR=${etl_DIR}
        -DETL_VERSION=${ETL_VERSION}
        -Dfsm-utils_DIR=${fsm-utils_DIR}
        -DFSM_UTILS_VERSION=${FSM_UTILS_VERSION}
        -Dtimer-utils_DIR=${timer-utils_DIR}
        -DTIMER_UTILS_VERSION=${TIMER_UTILS_VERSION}
        -DCXX_FLAGS=${CXX_FLAGS}
        -DFSM_VERBOSE_LOGGING=${FSM_VERBOSE_LOGGING}
    CMAKE_CACHE_ARGS
        -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    BUILD_ALWAYS
        1
)

install(
    DIRECTORY
        ${STAGED_INSTALL_PREFIX}/bin
        ${STAGED_INSTALL_PREFIX}/include
        ${STAGED_INSTALL_PREFIX}/lib
        ${STAGED_INSTALL_PREFIX}/share
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
    USE_SOURCE_PERMISSIONS
)
