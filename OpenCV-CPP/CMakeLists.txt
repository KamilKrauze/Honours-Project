cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_SUPPRESS_REGENERATION true)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

project(HonsProject VERSION 1.0)

# Add the source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add library directories
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Link against GLFW library
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)

# Organize source files into logical folders in Visual Studio

# You can add more libraries if needed, for example:
# target_link_libraries(HonsProjectExecutable opencv_core opencv_highgui)