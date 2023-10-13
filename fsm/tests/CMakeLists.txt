set(TEST_PROJECT_NAME ${PROJECT_NAME}-test)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(${TEST_PROJECT_NAME} 
    test_main.cpp
    test_fsm.cpp
)

target_link_libraries(${TEST_PROJECT_NAME}
    PRIVATE
    GTest::GTest
    fsm)

add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})
