cmake_minimum_required(VERSION 3.2)

project(basement_engine)

option(TESTS "Build basic tests" OFF)
option(BINDPY "Build with pybind11" OFF)
option(GLPATH "Search system for OpenGL" OFF)
option(GLEWPATH "Search system for GLEW" OFF)

if (NOT BINDPY)

	add_executable(basement)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread") 

else()
	
	#[[
	if (NOT EXISTS ${PYBINDPATH})
		message(FATAL_ERROR "Set a valid path for PYBINDPATH, ${PYBINDPATH} is invalid.")
	else()
		message(STATUS "Found pybind11")
	endif()
	]]

	add_library(basement SHARED)

	find_package(Python3 3.6 EXACT COMPONENTS Interpreter Development)

	#[[
	set(PYBIND11_FLAGS_CMD -m pybind11 --include)

	execute_process(COMMAND ${Python3_EXECUTABLE} ${PYBIND11_FLAGS_CMD}
					OUTPUT_VARIABLE PYBIND11_INCLUDE_DIRS
					WORKING_DIRECTORY ${PYBINDPATH} )

	string(REGEX REPLACE "-I|\n" "" PYBIND11_INCLUDE_DIRS ${PYBIND11_INCLUDE_DIRS})

	separate_arguments(PYBIND11_INCLUDE_DIRS)
	message(STATUS "Pybind11 include directories: ${PYBIND11_INCLUDE_DIRS}")
	target_include_directories( basement PUBLIC ${PYBIND11_INCLUDE_DIRS} )
	]]

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

	target_include_directories( basement PUBLIC ${Python3_INCLUDE_DIRS} )
	

	#target_link_options(basement PUBLIC -undefined dynamic_lookup)
	set_target_properties(basement PROPERTIES SUFFIX ".cpython-36m-x86_64-linux-gnu.so" PREFIX "" OUTPUT_NAME "GameContext")

	add_definitions(-DPYBINDMAIN)

endif()


set_property(TARGET basement PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
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


find_package(PkgConfig REQUIRED)


if (DEFINED MACOS)
	find_package(OpenCV REQUIRED)
else ()
	#find_package(opencv REQUIRED)
	pkg_check_modules(OpenCV REQUIRED opencv)
endif()

#include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
#message(STATUS "GLEW includes: ${GLEW_INCLUDE_DIRS}")
#message(STATUS "OPENGL includes: ${OPENGL_INCLUDE_DIRS}")

set(USINGMAC ON)
set(CD "${CMAKE_CURRENT_SOURCE_DIR}")

set(ASSIMP_LIB_PATH ${CD}/libs/assimp/build/bin/) 

set(PHYSX_LIBS_DIR ${CD}/libs/physx/PhysX_3.4/Lib/linux64/)
set(PHYSX_LIBS_DIR_SHARED ${CD}/libs/physx/PxShared/lib/linux64/)

set(PHYSX_INC_DIR ${CD}/libs/physx/PhysX_3.4/Include)
set(PHYSX_INC_DIR_SHARED ${CD}/libs/physx/PxShared/include)


file(GLOB PHYSX_LIBS "${PHYSX_LIBS_DIR}lib*CHECKED*.*")
file(GLOB PHYSX_LIBS_SHARED "${PHYSX_LIBS_DIR_SHARED}lib*CHECKED*.*")

message(STATUS "Libs found: ${PHYSX_LIBS}")
message(STATUS "Libs found (shared): ${PHYSX_LIBS_SHARED}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PHYSX_INC_DIR_SHARED})
include_directories(${PHYSX_INC_DIR})

add_subdirectory(src)
add_subdirectory(src/rendering)
add_subdirectory(src/mesh)
add_subdirectory(src/utils)
add_subdirectory(src/phys)
#add_subdirectory(libs/imgui)

#target_link_libraries(basement PUBLIC "-Wl,--allow-shlib-undefined")
target_link_libraries(basement PUBLIC "-Wl,--no-as-needed")  #<---this is needed on ubuntu

target_link_directories(basement PUBLIC libs/glm/build/glm)
target_link_libraries(basement PUBLIC glm_static)
target_link_libraries(basement PUBLIC -lpthread)

target_link_libraries(basement PUBLIC m)

target_link_libraries(basement PUBLIC ${PHYSX_LIBS_SHARED})
target_link_libraries(basement PUBLIC ${PHYSX_LIBS})

target_link_directories(basement PUBLIC ${OpenCV_LIBRARY_DIRS})
target_link_libraries(basement PUBLIC ${OpenCV_LIBRARIES})

target_link_directories(basement PUBLIC libs/glfw/build/src)
target_link_libraries(basement PUBLIC glfw3) #glfw.3 on mac

target_link_directories(basement PUBLIC ${ASSIMP_LIB_PATH})
target_link_libraries(basement PUBLIC IrrXML)
target_link_libraries(basement PUBLIC assimp)

target_link_libraries(basement PUBLIC z)
target_link_libraries(basement PUBLIC -ldl)
target_link_libraries(basement PUBLIC -lpthread)

