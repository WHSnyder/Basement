# Project definition
cmake_minimum_required(VERSION 3.1)

project(Rendering)

if (DEFINED PYBINDING)
	add_library(rendering SHARED Shader.cpp Rendertarget.cpp Texture.cpp)
else()
	target_sources(glTest PRIVATE Shader.cpp RenderTarget.cpp Texture.cpp)
endif()