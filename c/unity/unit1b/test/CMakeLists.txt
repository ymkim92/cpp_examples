add_executable(test_fib TestFib.c ../src/Fib.c)   
target_link_libraries(test_fib unity)   
target_include_directories(test_fib PUBLIC ../src ../unity/src)
add_test(NAME test_main COMMAND test_fib)
