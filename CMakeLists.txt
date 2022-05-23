cmake_minimum_required(VERSION 3.2)
project(fmt)

# include (cmake/utils.cmake)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS}\
    -rdynamic\
    -O3\
    -ggdb\
    -std=c++2a\
    -Wall\
    -Werror\
    -Wno-unused-function\
    -Wno-builtin-macro-redefined"
)

include_directories(./)


option(TESTS "fmt tests execute" OFF)

# redefine_file_macro(asco)
if(TESTS)
add_executable(test_format "tests/test_format.cc")

endif(TESTS)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
