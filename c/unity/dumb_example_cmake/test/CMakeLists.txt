add_executable(test_dumb_app TestDumbExample.c)   
target_link_libraries(test_dumb_app ../src ../unity/src)   
target_include_directories(test_dumb_app PUBLIC ../src ../unity/src)
add_test(test_dumb test_dumb_app)
