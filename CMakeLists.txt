cmake_minimum_required(VERSION 3.16)
project(lacam-project CXX)

add_subdirectory(./lacam)
add_subdirectory(./third_party/argparse)
add_subdirectory(./third_party/googletest)

add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_17)
target_link_libraries(main lacam argparse)
# Enable AddressSanitizer for main
target_compile_options(main PUBLIC -fsanitize=address)
target_link_options(main PUBLIC -fsanitize=address)

# test
set(TEST_MAIN_FUNC ./third_party/googletest/googletest/src/gtest_main.cc)
set(TEST_ALL_SRC ${TEST_MAIN_FUNC})
macro(add_test name target)
  list(APPEND TEST_ALL_SRC ${target})  
  add_executable(${name} ${target} ${TEST_MAIN_FUNC})
  target_link_libraries(${name} lacam gtest)
  # Enable AddressSanitizer for each test
  target_compile_options(${name} PUBLIC -fsanitize=address)
  target_link_options(${name} PUBLIC -fsanitize=address)
endmacro(add_test)

add_test(test_graph ./tests/test_graph.cpp)
add_test(test_instance ./tests/test_instance.cpp)
add_test(test_dist_table ./tests/test_dist_table.cpp)
add_test(test_planner ./tests/test_planner.cpp)
add_test(test_post_processing ./tests/test_post_processing.cpp)

add_executable(test_all ${TEST_ALL_SRC})
# Enable AddressSanitizer for test_all
target_compile_options(test_all PUBLIC -fsanitize=address)
target_link_options(test_all PUBLIC -fsanitize=address)
target_link_libraries(test_all lacam gtest)
