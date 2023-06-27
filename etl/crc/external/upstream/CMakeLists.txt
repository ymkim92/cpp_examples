macro(check_package project version share_location)
    find_package(${project} ${version} EXACT CONFIG QUIET)

    if(${project}_FOUND)
        message(STATUS "Found ${project} version ${${project}_VERSION}")
        add_library(${project}_external INTERFACE)  # dummy
    else()
        set(DEF_${project}_DIR ${STAGED_INSTALL_PREFIX}/share/${share_location})
        file(TO_NATIVE_PATH "${DEF_${project}_DIR}" DEF_${project}_DIR)
        set(${project}_DIR ${DEF_${project}_DIR}
            CACHE PATH "Path to internally built ${project}Config.cmake" FORCE)
    endif()
endmacro()

macro(build_external project)
    if(NOT ${project}_FOUND)
        add_subdirectory(${project})
    endif()
endmacro()

check_package(etl ${ETL_VERSION} etl/cmake)

build_external(etl)
