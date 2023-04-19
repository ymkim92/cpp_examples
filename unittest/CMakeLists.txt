cmake_minimum_required(VERSION 3.10)

project(CppExampleTests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(TEST_ROOT_DIR ..)
include_directories(${TEST_ROOT_DIR}/ringbuf/lwrb)
enable_testing()

add_executable(lwrbTests
            lwrbTest.cpp
            ${TEST_ROOT_DIR}/ringbuf/lwrb/lwrb.c)

# target_link_libraries(lwrbTests ${GTEST_LIBRARIES} pthread)
# https://stackoverflow.com/questions/8507723/how-to-start-working-with-gtest-and-cmake
target_link_libraries(lwrbTests gtest gtest_main pthread)

add_test(lwrbTests lwrbTests)