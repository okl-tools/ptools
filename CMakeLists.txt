cmake_minimum_required(VERSION 3.20)

project(ptools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (PTOOLS_LIB ptools)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


add_compile_definitions(PC_VERSION)

include_directories(inc)

file(GLOB_RECURSE PTOOLS_SRCS "src/*.cpp")

add_library (${PTOOLS_LIB} STATIC ${PTOOLS_SRCS} )

add_subdirectory(src_usage)
