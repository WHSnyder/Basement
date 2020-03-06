# Project definition
cmake_minimum_required(VERSION 3.1)

project(tests)

set ( CMAKE_CXX_STANDARD 11 )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable( test_opencv test_OPENCV.cpp )


target_link_libraries( test_opencv ${OpenCV_LIBS} )



