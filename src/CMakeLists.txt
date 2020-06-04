cmake_minimum_required(VERSION 3.2)
project(Core)


include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_sources(basement PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/GameContext.cpp")