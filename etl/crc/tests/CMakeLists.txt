cmake_minimum_required(VERSION 3.14.4)

set(TEST_PROJECT_NAME ${PROJECT_NAME}-test)

add_executable(${TEST_PROJECT_NAME}
    test_main.cpp
    test_crc.cpp
)

target_link_libraries(${TEST_PROJECT_NAME}
    PRIVATE
    ${PROJECT_NAME}-static
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    etl::etl
    )

target_include_directories(${TEST_PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../include
    ${CMAKE_BINARY_DIR}/${INSTALL_INCLUDEDIR}>
)

add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})

set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

gtest_discover_tests(${TEST_PROJECT_NAME})
