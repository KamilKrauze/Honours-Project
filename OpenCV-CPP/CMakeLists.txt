cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

project(HonsProject VERSION 1.0)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("lib")