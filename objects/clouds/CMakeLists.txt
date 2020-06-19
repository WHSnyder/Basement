cmake_minimum_required(VERSION 3.2)

project(clouds)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_sources(basement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Clouds.cpp)