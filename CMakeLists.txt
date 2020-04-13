cmake_minimum_required(VERSION 3.2)

project(basement_engine)

option(TESTS "Build basic tests" OFF)
option(PYBINDINGS "Build with pybind11" OFF)

add_executable(basement) 
set_property(TARGET basement PROPERTY CXX_STANDARD 17)
include_directories(basement PUBLIC include)
include_directories(basement PUBLIC libs)
include_directories(basement PRIVATE libs/glm)
include_directories(basement PRIVATE libs/glfw/include)

target_link_libraries(basement PRIVATE libs/glm/build/glm/libglm_static.a)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)

set(USINGMAC ON)
if (USINGMAC)
	set(PHYSX_LIBS_DIR /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64/)
	set(PHYSX_LIBS_DIR_SHARED /Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64/)

	set(PHYSX_INC_DIR /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include)
	set(PHYSX_INC_DIR_SHARED /Users/will/projects/cpprtx/libs/physx/PxShared/include)
endif()

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

target_link_libraries(basement PRIVATE z)
target_link_libraries(basement PUBLIC ${PHYSX_LIBS})
target_link_libraries(basement PUBLIC ${PHYSX_LIBS_SHARED})
target_link_libraries(basement PRIVATE ${OpenCV_LIBS})
target_link_libraries(basement PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_link_directories(basement PRIVATE libs/glfw/build/src)
target_link_libraries(basement PRIVATE glfw.3)
target_link_libraries(basement PRIVATE "${ASSIMP_LIB_PATH}libIrrXML.a")
target_link_libraries(basement PRIVATE "${ASSIMP_LIB_PATH}libassimp.a")