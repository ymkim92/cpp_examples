cmake_minimum_required(VERSION 3.10)

project(Thread)

find_package(Boost 1.67.0 COMPONENTS date_time thread)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_executable(main main.cpp task1.cpp)
    target_link_libraries(main ${Boost_LIBRARIES})
endif()