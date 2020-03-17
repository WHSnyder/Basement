# Project definition
cmake_minimum_required(VERSION 3.1)

set(UTILS_SOURCES ShaderUtils.cpp controls.cpp)

if (PYBINDING)
	add_library(utils SHARED ${UTILS_SOURCES})
else ()
	target_sources(glTest PRIVATE ${UTILS_SOURCES})
endif()
