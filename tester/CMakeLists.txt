include_directories(./)
file(GLOB TESTER_SOURCES  "./*.cpp")
file(GLOB TESTER_PARAMS  "./argument/*.cpp")
add_library(Tester ${TESTER_SOURCES} ${TESTER_PARAMS})
