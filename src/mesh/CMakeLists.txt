# Project definition
cmake_minimum_required(VERSION 3.1)

project(Mesh)

#-I/Users/will/projects/cpprtx/libs/assimp/include/ -I/Users/will/projects/cpprtx/libs/assimp/build/include/ -lz 
#-lIrrXML -lassimp

set(ASSIMP_LIB_PATH /Users/will/projects/cpprtx/libs/assimp/build/lib/) 

include_directories(/Users/will/projects/cpprtx/libs/assimp/include/)
include_directories(/Users/will/projects/cpprtx/libs/assimp/build/include/)

if (DEFINED PYBINDING)
	add_library(mesh SHARED Mesh.cpp)
else()
	target_link_libraries(glTest PRIVATE z)
	target_link_libraries(glTest PRIVATE "${ASSIMP_LIB_PATH}libIrrXML.a")
	target_link_libraries(glTest PRIVATE "${ASSIMP_LIB_PATH}libassimp.a")
	target_sources(glTest PRIVATE Mesh.cpp)
endif()