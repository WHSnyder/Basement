# Project definition
cmake_minimum_required(VERSION 3.1)

project(mesh)

#-I/Users/will/projects/cpprtx/libs/assimp/include/ -I/Users/will/projects/cpprtx/libs/assimp/build/include/ -lz 
#-lIrrXML -lassimp

set(ASSIMP_LIB_PATH /Users/will/projects/cpprtx/libs/assimp/build/lib/) 

include_directories(/Users/will/projects/cpprtx/libs/assimp/include/)
include_directories(/Users/will/projects/cpprtx/libs/assimp/build/include/)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

if (DEFINED PYBIND)
	add_library(mesh SHARED Mesh.cpp)
else()
	target_sources(basement PRIVATE Mesh.cpp)
endif()