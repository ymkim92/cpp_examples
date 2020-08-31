cmake_minimum_required(VERSION 3.10)

project(LedControl)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(ledcontrol main.c ledcontrol.c)
target_link_libraries(ledcontrol PRIVATE Threads::Threads)
