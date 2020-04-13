# Project definition
cmake_minimum_required(VERSION 3.1)

set(UTILS_SOURCES ShaderUtils.cpp Controls.cpp)

if (PYBINDING)
	add_library(utils SHARED ${UTILS_SOURCES})
else ()
	include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
	target_sources(basement PRIVATE ${UTILS_SOURCES})
endif()
