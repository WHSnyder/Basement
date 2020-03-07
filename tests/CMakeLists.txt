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
message(STATUS "Python exec found at ${Python3_EXECUTABLE}")
message(STATUS "Python includes found at ${Python3_INCLUDE_DIRS}")



set(PYBIND11_FLAGS_CMD_ -m pybind11 --include)

execute_process(COMMAND ${Python3_EXECUTABLE} ${PYBIND11_FLAGS_CMD_}
				OUTPUT_VARIABLE PYBIND11_FLAGS_
				WORKING_DIRECTORY ${PYBIND11_INCS} )

message(STATUS "pybind11 include flags: ${PYBIND11_FLAGS_}")


#[[
Test command

clang++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `PYTHONPATH=/Users/will/projects/py_scripting/pybind11 python3 -m pybind11 --includes` test_OPENCV.cpp -o test_OPENCV`python3-config --extension-suffix`
]]


set(PYBIND11_FLAGS "/Users/will/projects/py_scripting/pybind11/build/mock_install/include")
set(PYBIND11_MDIR "/usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/include/python3.7m")


set ( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "-O3" )


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )



if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(UNDEF_LOOKUP "-undefined dynamic_lookup")
endif()



target_include_directories( test_OPENCV PUBLIC ${Python3_INCLUDE_DIRS} )
target_include_directories( test_OPENCV PUBLIC ${PYBIND11_FLAGS} )
target_include_directories( test_OPENCV PUBLIC ${PYBIND11_MDIR} )


#target_compile_features( test_OPENCV PUBLIC ${UNDEF_LOOKUP} )
#target_link_options( test_OPENCV PUBLIC ${UNDEF_LOOKUP} )

target_link_libraries( test_OPENCV ${OpenCV_LIBS} )
target_link_libraries( test_OPENCV "-undefined dynamic_lookup" )

set_target_properties( test_OPENCV PROPERTIES 
						SUFFIX ".cpython-37m-darwin.so" 
						PREFIX ""
						)
