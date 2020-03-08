# Project definition
cmake_minimum_required(VERSION 3.1)

project(Core)

add_subdirectory(rendering)
add_subdirectory(mesh)
add_subdirectory(utils)
add_subdirectory(phys)

add_executable(gltest, glTest.cpp)
