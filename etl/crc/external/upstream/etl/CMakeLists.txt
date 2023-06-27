message(STATUS "Suitable etl could not be located, Building etl instead.")

include(ExternalProject)
ExternalProject_Add(etl_external
    GIT_REPOSITORY
        # ssh://git@git.skybornetech.com:10022/mirrors/etl-cpp.git
        https://github.com/ETLCPP/etl.git
    GIT_TAG
        ${ETL_GIT_TAG}
    UPDATE_COMMAND
        ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
        -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    CMAKE_CACHE_ARGS
        -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    DOWNLOAD_NO_PROGRESS 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
)

message(STATUS "etl done")