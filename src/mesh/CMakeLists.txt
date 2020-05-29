# Project definition
cmake_minimum_required(VERSION 3.1)

project(mesh)

target_include_directories(basement PRIVATE /Users/will/projects/cpprtx/libs/assimp/include/)
target_include_directories(basement PRIVATE /Users/will/projects/cpprtx/libs/assimp/build/include/)

if (DEFINED PYBIND)
	add_library(mesh SHARED Mesh.cpp)
else()
	target_sources(basement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp)
endif()