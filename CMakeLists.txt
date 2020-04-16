cmake_minimum_required(VERSION 3.2)

project(basement_engine)

option(TESTS "Build basic tests" OFF)
option(PYBINDPATH "Build with pybind11" OFF)
option(GLPATH "Search system for OpenGL" OFF)
option(GLEWPATH "Search system for GLEW" OFF)

if (NOT PYBINDPATH)

	add_executable(basement) 

else()
	
	if (NOT EXISTS ${PYBINDPATH})
		message(FATAL_ERROR "Set a valid path for PYBINDPATH, ${PYBINDPATH} is invalid.")
	else()
		message(STATUS "Found pybind11")
	endif()

	add_library(pybind_interface INTERFACE)

	find_package(Python3 3.7 EXACT COMPONENTS Interpreter Development)

	set(PYBIND11_FLAGS_CMD -m pybind11 --include)

	execute_process(COMMAND ${Python3_EXECUTABLE} ${PYBIND11_FLAGS_CMD}
					OUTPUT_VARIABLE PYBIND11_INCLUDE_DIRS
					WORKING_DIRECTORY ${PYBINDPATH} )

	string(REGEX REPLACE "-I|\n" "" PYBIND11_INCLUDE_DIRS ${PYBIND11_INCLUDE_DIRS})

	separate_arguments(PYBIND11_INCLUDE_DIRS)
	message(STATUS "Pybind11 include directories: ${PYBIND11_INCLUDE_DIRS}")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

	target_include_directories(pybind_interface INTERFACE ${Python3_INCLUDE_DIRS})
	target_include_directories(pybind_interface INTERFACE ${PYBIND11_INCLUDE_DIRS})
	target_link_options(pybind_interface INTERFACE -undefined dynamic_lookup)
	target_compile_options(pybind_interface INTERFACE -DPYBINDMAIN)
	#add_definitions(-DPYBINDMAIN)


	add_library(basement SHARED)
	target_link_libraries(basement INTERFACE pybind_interface)
	set_target_properties(basement PROPERTIES SUFFIX ".cpython-37m-darwin.so" PREFIX "" OUTPUT_NAME "GameContext")

endif()


set_property(TARGET basement PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-everything -fPIC -lpthread")
include_directories(basement PUBLIC include)
include_directories(basement PUBLIC libs)
include_directories(basement PRIVATE libs/glm)
include_directories(basement PRIVATE libs/glfw/include)
include_directories(basement PRIVATE /usr/local/include)

if (DEFINED GLPATH)
	target_link_directories(basement PRIVATE ${GLPATH})
	target_link_libraries(basement PRIVATE GL GLEW)
else()
	find_package(OpenGL REQUIRED)
endif()

find_package(OpenCV REQUIRED)

#include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
#message(STATUS "GLEW includes: ${GLEW_INCLUDE_DIRS}")
#message(STATUS "OPENGL includes: ${OPENGL_INCLUDE_DIRS}")

set(USINGMAC ON)
set(CD "${CMAKE_CURRENT_SOURCE_DIR}")

set(ASSIMP_LIB_PATH ${CD}/libs/assimp/build/lib/) 

set(PHYSX_LIBS_DIR ${CD}/libs/physx/PhysX_3.4/Lib/osx64/)
set(PHYSX_LIBS_DIR_SHARED ${CD}/libs/physx/PxShared/lib/osx64/)

set(PHYSX_INC_DIR ${CD}/libs/physx/PhysX_3.4/Include)
set(PHYSX_INC_DIR_SHARED ${CD}/libs/physx/PxShared/include)


file(GLOB PHYSX_LIBS "${PHYSX_LIBS_DIR}lib*CHECKED.a")
file(GLOB PHYSX_LIBS_SHARED ${PHYSX_LIBS_DIR_SHARED}lib*CHECKED.a)

message(STATUS "Libs found: ${PHYSX_LIBS}")
message(STATUS "Libs found (shared): ${PHYSX_LIBS_SHARED}")

add_subdirectory(src)
add_subdirectory(src/rendering)
add_subdirectory(src/mesh)
add_subdirectory(src/utils)
add_subdirectory(src/phys)
add_subdirectory(libs/imgui)

target_link_directories(basement PRIVATE libs/glm/build/glm)
target_link_libraries(basement PRIVATE glm_static)

target_link_libraries(basement PRIVATE z)
target_link_libraries(basement PUBLIC ${PHYSX_LIBS})
target_link_libraries(basement PUBLIC ${PHYSX_LIBS_SHARED})
target_link_libraries(basement PRIVATE ${OpenCV_LIBS})
target_link_directories(basement PRIVATE libs/glfw/build/src)
target_link_libraries(basement PRIVATE glfw.3)
target_link_libraries(basement PRIVATE "${ASSIMP_LIB_PATH}/libIrrXML.a")
target_link_libraries(basement PRIVATE "${ASSIMP_LIB_PATH}/libassimp.a")