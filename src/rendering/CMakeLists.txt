# Project definition
cmake_minimum_required(VERSION 3.2)

project(rendering)

if (DEFINED PYBINDING)
	add_library(rendering SHARED Shader.cpp Rendertarget.cpp Texture.cpp)
else()
	include_directories(${OpenCV_INCLUDE_DIRS})
	include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
	target_sources(basement PRIVATE Shader.cpp RenderTarget.cpp Texture.cpp)
endif()