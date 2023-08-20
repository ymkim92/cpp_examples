cmake_minimum_required(VERSION 3.10)

project(epoll_test_pointer)

FIND_PACKAGE(PkgConfig REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(epoll_ptr main.c )
target_link_libraries(epoll_ptr PRIVATE Threads::Threads)
