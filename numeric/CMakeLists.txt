include_directories(./)
file(GLOB NUMERIC_SOURCES  "./*.cpp")
file(GLOB NUMERIC_MODULES  "./arithm/*.cpp")
add_library(Numeric ${NUMERIC_SOURCES} ${NUMERIC_MODULES})

