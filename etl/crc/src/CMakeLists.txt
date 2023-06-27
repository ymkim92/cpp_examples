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
    ${PROJECT_SOURCE_DIR}/main.cpp
)

# Create object library so source is only compiled once
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_options(${PROJECT_NAME}
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
    target_compile_options(${PROJECT_NAME}
        INTERFACE
            -Wno-error=stringop-truncation
            -Wno-error=address-of-packed-member
    )
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # Boost::boost
        etl::etl
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Release>:-flto>
        $<$<CONFIG:Release>:-ffat-lto-objects>
        $<$<CONFIG:Release>:-s>
)

install(
    TARGETS
        ${PROJECT_NAME}
    DESTINATION
        ${INSTALL_CMAKEDIR}
)
