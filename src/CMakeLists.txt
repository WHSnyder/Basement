# Project definition
cmake_minimum_required(VERSION 3.17)

project(Core)

add_executable(glTest glTest.cpp)
set_property(TARGET glTest PROPERTY CXX_STANDARD 17)

set(USINGMAC "on")

if (NOT DEFINED GLM_LIB_DIR OR NOT DEFINED GLM_INC_DIR)
	message(FATAL_ERROR "Supply binary and include paths (GLM_LIB_DIR/GLM_INC_DIR) to glm!")
endif()


if (DEFINED PYBIND)
	add_compile_definitions(PYBIND)
	if (NOT DEFINED PYBIND11_INCS OR NOT EXISTS ${PYBIND11_INCS})
		message(FATAL_ERROR "Set a valid path for PYBIND11_INCS")
	else ()
		message(STATUS "Found pybind11")
	endif()
endif()


target_link_libraries(libs/glm/build/libglm_static.a)


add_subdirectory(rendering)
add_subdirectory(mesh)
add_subdirectory(utils)
add_subdirectory(phys)

#If were compiling for Python bindings, treat subdirs as sublibs
if (DEFINED PYBIND)
	target_link_libraries(gltest PRIVATE phys)
	target_link_libraries(gltest PRIVATE rendering)
	target_link_libraries(gltest PRIVATE utils)
	target_link_libraries(gltest PRIVATE mesh)
	target_link_libraries(gltest PRIVATE -undefined dynamic_lookup)
else()