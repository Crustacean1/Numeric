include_directories(./)
file(GLOB NUMERIC_SOURCES  "./*.cpp")
file(GLOB NUMERIC_MODULES  "./Arithm/*.cpp")
file(GLOB NUMERIC_BUFFERS  "./Buffer/*.cpp")
add_library(Numeric ${NUMERIC_SOURCES} ${NUMERIC_MODULES} ${NUMERIC_BUFFERS})

