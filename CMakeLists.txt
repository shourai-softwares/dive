#CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Dive)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()

find_package(OpenGL REQUIRED)

set (Dive_VERSION_MAJOR 0)
set (Dive_VERSION_MINOR 1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable(dive ${SOURCES})
target_link_libraries(dive glfw GLEW tinyobjloader ${OPENGL_LIBRARY})
