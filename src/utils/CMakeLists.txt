# Project definition
cmake_minimum_required(VERSION 3.1)

set(UTILS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ShaderUtils.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Controls.cpp)

if (PYBINDING)
	add_library(utils SHARED ${UTILS_SOURCES})
else ()
	include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
	target_sources(basement PRIVATE ${UTILS_SOURCES})
endif()
