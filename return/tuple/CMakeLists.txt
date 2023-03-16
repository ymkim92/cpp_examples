cmake_minimum_required(VERSION 3.10)

project(tupletest)

message(${PROJECT_NAME})
add_executable(${PROJECT_NAME} tuple.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
