cmake_minimum_required(VERSION 3.10)

project(Numeric VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(numeric)
add_subdirectory(tester)

add_executable(NumericTester main.cpp tests/arithm_tests.cpp)

target_link_libraries(NumericTester PUBLIC Numeric)
target_link_libraries(NumericTester PUBLIC Tester)

target_include_directories(Tester PUBLIC
  ${PROJECT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}/numeric
  ${PROJECT_BINARY_DIR}/tester
  ${PROJECT_BINARY_DIR}/tests)
