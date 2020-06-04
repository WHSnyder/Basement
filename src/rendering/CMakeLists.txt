# Project definition
cmake_minimum_required(VERSION 3.2)

project(rendering)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_sources(basement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Texture.cpp)