# Project definition
cmake_minimum_required(VERSION 3.1)

project(engine)

option(TESTS "Build basic tests" OFF)
option(PYBINDINGS "Build with pybind11" OFF)

include_directories("src")
include_directories("libs")

if (TESTS)
	add_subdirectory(tests)
endif()

add_subdirectory(src)