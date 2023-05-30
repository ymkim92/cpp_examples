add_executable(test_dumb_app TestDumbExample.c ../src/DumbExample.c)   
target_link_libraries(test_dumb_app unity)   
target_include_directories(test_dumb_app PUBLIC ../src ../unity/src)
add_test(test_dumb test_dumb_app)
