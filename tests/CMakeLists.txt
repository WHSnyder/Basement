# Project definition
cmake_minimum_required(VERSION 3.2)

project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")



#BUILD PYBIND11 OPENCV/NUMPY TESTS

add_library(test_OPENCV SHARED test_OPENCV.cpp)

find_package(Python3 3.7 EXACT COMPONENTS Interpreter Development)
target_include_directories(test_OPENCV PUBLIC ${Python3_INCLUDE_DIRS})

execute_process (
	COMMAND python3-config "--extension-suffix"
	OUTPUT_VARIABLE PY_EXT_SUFFIX
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

set_target_properties(
	test_OPENCV PROPERTIES 
	SUFFIX "${PY_EXT_SUFFIX}"
	PREFIX ""
	OUTPUT_NAME "test_OPENCV"
)

find_package(OpenCV REQUIRED)

target_include_directories(test_OPENCV PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(test_OPENCV PUBLIC ${Python3_INCLUDE_DIRS})

target_link_libraries(test_OPENCV ${OpenCV_LIBS})

if (APPLE)
	target_link_options(test_OPENCV PUBLIC -undefined dynamic_lookup)
endif()



#BUILD METAL TESTS

if (APPLE)
	add_executable(test_METAL test_MetalGLFW.mm)
	message(STATUS "GLFW path: ${GLFW_LIB_PATH}")
	target_link_directories(test_METAL PUBLIC ${GLFW_LIB_PATH})
	target_link_libraries(test_METAL PUBLIC glfw.3)
	target_link_options(test_METAL PUBLIC -undefined dynamic_lookup)
	target_link_libraries(test_METAL PUBLIC "-framework AppKit" "-framework Metal" "-framework QuartzCore")
endif()




