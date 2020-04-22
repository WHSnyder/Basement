cmake_minimum_required(VERSION 3.2)
project(Core)


include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_sources(basement PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/GameContext.cpp")


if (DEFINED PYBINDLOL)
	add_compile_definitions(PYBIND)
	if (NOT DEFINED PYBIND11_INCS OR NOT EXISTS ${PYBIND11_INCS})
		message(FATAL_ERROR "Set a valid path for PYBIND11_INCS")
	else ()
		message(STATUS "Found pybind11")
	endif()

	target_link_libraries(gltest PRIVATE phys)
	target_link_libraries(gltest PRIVATE rendering)
	target_link_libraries(gltest PRIVATE utils)
	target_link_libraries(gltest PRIVATE mesh)
	target_link_libraries(gltest PRIVATE -undefined dynamic_lookup)
endif()