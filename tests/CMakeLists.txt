# Project definition
cmake_minimum_required(VERSION 3.1)

project(test_opencv)
add_library( test_OPENCV SHARED test_OPENCV.cpp )


if (NOT DEFINED PYBIND11_INCS OR NOT EXISTS ${PYBIND11_INCS})
	message(FATAL_ERROR "Set a valid path for PYBIND11_INCS")
else ()
	message(STATUS "Found pybind11")
endif()


find_package(Python3 COMPONENTS Interpreter Development)

set(PYBIND11_FLAGS_CMD -m pybind11 --include)

execute_process(COMMAND ${Python3_EXECUTABLE} ${PYBIND11_FLAGS_CMD}
				OUTPUT_VARIABLE PYBIND11_INCLUDE_DIRS
				WORKING_DIRECTORY ${PYBIND11_INCS} )

string(REGEX REPLACE "-I|\n" "" PYBIND11_INCLUDE_DIRS ${PYBIND11_INCLUDE_DIRS})


separate_arguments(PYBIND11_INCLUDE_DIRS)
message(STATUS "Pybind11 include directories: ${PYBIND11_INCLUDE_DIRS}")

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-O3" )


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_include_directories( test_OPENCV PUBLIC ${Python3_INCLUDE_DIRS} )
target_include_directories( test_OPENCV PUBLIC ${PYBIND11_INCLUDE_DIRS} )

target_link_libraries( test_OPENCV ${OpenCV_LIBS} )
target_link_options( test_OPENCV PUBLIC -undefined dynamic_lookup )

set_target_properties( test_OPENCV PROPERTIES 
						SUFFIX ".cpython-37m-darwin.so" 
						PREFIX "" )