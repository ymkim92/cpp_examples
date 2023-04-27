cmake_minimum_required(VERSION 3.10)

project(CppExampleTests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# TODO check if this is necessary
find_package(GTest REQUIRED CONFIG)

set(TEST_ROOT_DIR ..)
include_directories(${TEST_ROOT_DIR}/ringbuf/lwrb)
enable_testing()

add_executable(lwrbTests
            lwrbTest.cpp
            ${TEST_ROOT_DIR}/ringbuf/lwrb/lwrb.c)

# target_link_libraries(lwrbTests ${GTEST_LIBRARIES} pthread)
# https://stackoverflow.com/questions/8507723/how-to-start-working-with-gtest-and-cmake
target_link_libraries(lwrbTests gtest gtest_main pthread)

# add_test(lwrbTests lwrbTests)
include(GoogleTest)
gtest_discover_tests(lwrbTests)