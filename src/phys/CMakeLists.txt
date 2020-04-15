# Project definition
cmake_minimum_required(VERSION 3.1)

project(phys)

if (PYBIND)
	add_library(phys STATIC Physics.cpp)

	target_include_directories(phys PUBLIC ${PHYSX_INC_DIR})
	target_include_directories(phys PUBLIC ${PHYSX_INC_DIR_SHARED})

	target_link_libraries(phys PUBLIC ${PHYSX_LIBS})
	target_link_libraries(phys PUBLIC ${PHYSX_LIBS_SHARED})
	#target_link_libraries(phys PUBLIC -Llibs/glm/build/ -lglm_static)
else ()
	target_include_directories(basement PUBLIC ${PHYSX_INC_DIR_SHARED} ${PHYSX_INC_DIR})
	target_sources(basement PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Physics.cpp )
endif()