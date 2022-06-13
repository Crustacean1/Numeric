include_directories(./)
file(GLOB NUMERIC_SOURCES  "./*.cpp")
file(GLOB NUMERIC_MODULES  "./Arithm/*.cpp")
file(GLOB NUMERIC_BUFFERS  "./Buffer/*.cpp")
add_compile_options(-O3)
add_library(numeric ${NUMERIC_SOURCES} ${NUMERIC_MODULES} ${NUMERIC_BUFFERS})

