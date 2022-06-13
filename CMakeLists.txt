enable_testing()

cmake_minimum_required(VERSION 3.10)

function(do_test target arg)
  add_test(NAME ${arg} COMMAND ${target} "${PROJECT_SOURCE_DIR}/testfiles/${arg}")
  set_tests_properties(${arg}
    PROPERTIES WILL_FAIL FALSE )
endfunction()

project(Numeric VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(numeric)
add_subdirectory(tester)

include_directories(./)
file(GLOB TEST_SOURCES  "./tests/*.cpp")
file(GLOB TEST_PARAMS  "./tests/ArgumentGenerator/*.cpp")

add_executable(NumericTester main.cpp ${TEST_SOURCES} ${TEST_PARAMS})

target_link_libraries(NumericTester PUBLIC numeric)
target_link_libraries(NumericTester PUBLIC Tester)

do_test("NumericTester" "addition")
do_test("NumericTester" "mul")
do_test("NumericTester" "shift")

target_include_directories(Tester PUBLIC
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/numeric
  ${PROJECT_BINARY_DIR}/tester
  ${PROJECT_BINARY_DIR}/tests)
