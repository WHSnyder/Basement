cmake_minimum_required(VERSION 3.2)

project(basement_engine)

option(TESTS "Build basic tests" OFF)
option(BINDPY "Build with pybind11" OFF)
option(GLPATH "Search system for OpenGL" OFF)

set(USINGMAC ON)
set(CD "${CMAKE_CURRENT_SOURCE_DIR}")

if (NOT BINDPY)

	add_executable(basement)

else()
	
	add_library(basement SHARED)

	find_package(Python3 3.6 EXACT COMPONENTS Interpreter Development)
	target_include_directories(basement PUBLIC ${Python3_INCLUDE_DIRS})
	
	execute_process (
    	COMMAND python3-config "--extension-suffix"
    	OUTPUT_VARIABLE PY_EXT_SUFFIX
    	OUTPUT_STRIP_TRAILING_WHITESPACE
    )

	set_target_properties(
		basement PROPERTIES 
		SUFFIX "${PY_EXT_SUFFIX}"
		PREFIX ""
		OUTPUT_NAME "GameContext"
	)

	add_definitions(-DPYBINDMAIN)

endif()

set_property(TARGET basement PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


if (UNIX AND NOT APPLE)
	target_link_libraries(basement PUBLIC "-Wl,--no-as-needed")
elseif(APPLE)
	target_link_options(basement PUBLIC -undefined dynamic_lookup)
endif()

include_directories(basement PUBLIC include)
include_directories(basement PUBLIC libs)
include_directories(basement PRIVATE libs/glm)
include_directories(basement PRIVATE libs/glfw/include)
include_directories(basement PRIVATE /usr/local/include)

if (DEFINED GLPATH)
	target_link_directories(basement PRIVATE ${GLPATH})
	target_link_libraries(basement PRIVATE GL GLEW)
else()
	message(ERROR "You must specify a path to your OpenGL shared objects/dylibs")
endif()

if (APPLE)
	find_package(OpenCV REQUIRED)
else ()
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(OpenCV REQUIRED opencv)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

set(ASSIMP_LIB_PATH ${CD}/libs/assimp/build/bin/) 

if (UNIX AND NOT APPLE)
	set(PX_DIR linux64/)
elseif(APPLE)
	set(PX_DIR osx64/)
endif()

#Physx SOs were moved into Lib folders from bin ones
set(PHYSX_LIBS_DIR ${CD}/libs/physx/PhysX_3.4/Lib/${PX_DIR})
set(PHYSX_LIBS_DIR_SHARED ${CD}/libs/physx/PxShared/lib/${PX_DIR})
set(PHYSX_INC_DIR ${CD}/libs/physx/PhysX_3.4/Include)
set(PHYSX_INC_DIR_SHARED ${CD}/libs/physx/PxShared/include)

file(GLOB PHYSX_LIBS "${PHYSX_LIBS_DIR}lib*DEBUG*.*")
file(GLOB PHYSX_LIBS_SHARED "${PHYSX_LIBS_DIR_SHARED}lib*DEBUG*.*")

message(STATUS "Physx libs found: ${PHYSX_LIBS}")
message(STATUS "Physx common libs found: ${PHYSX_LIBS_SHARED}")

include_directories(${PHYSX_INC_DIR_SHARED})
include_directories(${PHYSX_INC_DIR})

add_subdirectory(src)
add_subdirectory(src/rendering)
add_subdirectory(src/mesh)
add_subdirectory(src/utils)
add_subdirectory(src/phys)
#add_subdirectory(libs/imgui)

target_link_directories(basement PUBLIC libs/glm/build/glm)
target_link_libraries(basement PUBLIC glm_static)

target_link_libraries(basement PUBLIC ${PHYSX_LIBS_SHARED})
target_link_libraries(basement PUBLIC ${PHYSX_LIBS})

target_link_directories(basement PUBLIC ${OpenCV_LIBRARY_DIRS})
target_link_libraries(basement PUBLIC ${OpenCV_LIBRARIES})

target_link_directories(basement PUBLIC libs/glfw/build/src)

if (APPLE)
	target_link_libraries(basement PUBLIC glfw.3)
else()
	target_link_libraries(basement PUBLIC glfw3) 
endif()

target_link_directories(basement PUBLIC ${ASSIMP_LIB_PATH})
target_link_libraries(basement PUBLIC IrrXML)
target_link_libraries(basement PUBLIC assimp)
target_link_libraries(basement PUBLIC z)

if (UNIX AND NOT APPLE)
	target_link_libraries(basement PUBLIC -ldl) #handled automatically on Mac
endif()

target_link_libraries(basement PUBLIC -lpthread)
target_link_libraries(basement PUBLIC m)