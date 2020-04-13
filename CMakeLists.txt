cmake_minimum_required(VERSION 3.1)

project(basement)

option(TESTS "Build basic tests" OFF)
option(PYBINDINGS "Build with pybind11" OFF)

include_directories("include")
include_directories("libs")

if (TESTS)
	add_subdirectory(tests)
endif()

add_subdirectory(src)