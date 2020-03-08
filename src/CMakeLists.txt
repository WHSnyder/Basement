# Project definition
cmake_minimum_required(VERSION 3.1)

project(Core)

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


include_directories("src")
include_directories("libs")
include_directories("libs/glm")

add_subdirectory(rendering)
add_subdirectory(mesh)
add_subdirectory(utils)
add_subdirectory(phys)


add_executable(gltest, glTest.cpp)
target_link_libraries(gltest PRIVATE phys)
target_link_libraries(gltest PRIVATE rendering)
target_link_libraries(gltest PRIVATE utils)
target_link_libraries(gltest PRIVATE mesh)
target_link_libraries(gltest PRIVATE -undefined dynamic_lookup)