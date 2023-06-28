cmake_minimum_required(VERSION 3.14.4)

project(crc VERSION 1.0.0)
find_package(etl ${ETL_VERSION} EXACT CONFIG REQUIRED)
message(STATUS "Found etl version: ${etl_VERSION}")


include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/crc.c
    ${PROJECT_SOURCE_DIR}/etl_wrapper.cpp
)

set(INCLUDE_PUBLIC_HEADERS
)

# Create object library so source is only compiled once
add_library(${PROJECT_NAME}-objlib
    OBJECT
    ${SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME}-objlib
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS ON
    C_STANDARD 11
    C_EXTENSIONS ON
    POSITION_INDEPENDENT_CODE True
)

target_include_directories(${PROJECT_NAME}-objlib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}>
        $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/../include
)

target_compile_options(${PROJECT_NAME}-objlib
    PRIVATE
        -Wall
        -Wextra
        -Wfatal-errors
        -Werror
        -Wno-deprecated-declarations
        $<IF:$<CONFIG:Debug>,-Og,-O3>
        $<$<CONFIG:Debug>:-ggdb>
        $<$<CONFIG:Release>:-flto>
        $<$<CONFIG:Release>:-ffat-lto-objects>
)

# add further warnings if supported by compiler
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.1)
    target_compile_options(${PROJECT_NAME}-objlib
        INTERFACE
            -Wno-error=stringop-truncation
            -Wno-error=address-of-packed-member
    )
endif()

# target_compile_definitions(${PROJECT_NAME}-objlib
#     PRIVATE BOOST_ERROR_CODE_HEADER_ONLY BOOST_SYSTEM_NO_DEPRECATED BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
# )

# if (DEFINED FSM_VERBOSE_LOGGING)
# target_compile_definitions(${PROJECT_NAME}-objlib
#     PRIVATE FSM_VERBOSE_LOGGING
# )
# endif()

target_link_libraries(${PROJECT_NAME}-objlib
    PRIVATE
        etl::etl
)

target_link_options(${PROJECT_NAME}-objlib
    PRIVATE
        $<$<CONFIG:Release>:-flto>
        $<$<CONFIG:Release>:-ffat-lto-objects>
        $<$<CONFIG:Release>:-s>
)

# SHARED library
add_library(${PROJECT_NAME}-shared
    SHARED
    $<TARGET_OBJECTS:${PROJECT_NAME}-objlib>
)

set_target_properties(${PROJECT_NAME}-shared
    PROPERTIES
    PUBLIC_HEADER "${INCLUDE_PUBLIC_HEADERS}"
)

# STATIC library
add_library(${PROJECT_NAME}-static
    STATIC
    $<TARGET_OBJECTS:${PROJECT_NAME}-objlib>
)

set_target_properties(${PROJECT_NAME}-static
    PROPERTIES
    PUBLIC_HEADER "${INCLUDE_PUBLIC_HEADERS}"
)

install(
    TARGETS
        ${PROJECT_NAME}-shared
        ${PROJECT_NAME}-static
    EXPORT
        ${PROJECT_NAME}Targets
    LIBRARY
        DESTINATION ${INSTALL_LIBDIR}
        COMPONENT lib
    PUBLIC_HEADER
        DESTINATION ${INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT dev
)

install(
    EXPORT
        ${PROJECT_NAME}Targets
    NAMESPACE
        "${PROJECT_NAME}::"
    DESTINATION
        ${INSTALL_CMAKEDIR}
    COMPONENT
        dev
)

# Tests
if (ENABLE_TESTS)
    find_package(GTest REQUIRED CONFIG)
    include(GoogleTest)
    enable_testing()
    add_subdirectory(../tests ${CMAKE_CURRENT_BINARY_DIR}/tests)
    target_compile_options(${PROJECT_NAME}-objlib
        PRIVATE -fprofile-arcs -ftest-coverage
    )
    # The unit test app links against the static lib
    target_link_libraries(${PROJECT_NAME}-static
        PRIVATE gcov
    )
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/../cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
        ${INSTALL_CMAKEDIR}
)
