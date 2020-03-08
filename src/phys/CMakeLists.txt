# Project definition
cmake_minimum_required(VERSION 3.1)

project(Phys)
add_library(phys STATIC Physics.cpp)

set(CMAKE_CXX_STANDARD 11)
set(USINGMAC "yolo")


if (DEFINED USINGMAC)

	set(PHYSX_LIBS_DIR /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Lib/osx64/)
	set(PHYSX_LIBS_DIR_SHARED /Users/will/projects/cpprtx/libs/physx/PxShared/lib/osx64/)

	set(PHYSX_INC_DIR /Users/will/projects/cpprtx/libs/physx/PhysX_3.4/Include)
	set(PHYSX_INC_DIR_SHARED /Users/will/projects/cpprtx/libs/physx/PxShared/include)

endif()


file(GLOB PHYSX_LIBS "${PHYSX_LIBS_DIR}lib*CHECKED.a")
file(GLOB PHYSX_LIBS_SHARED ${PHYSX_LIBS_DIR_SHARED}lib*CHECKED.a)

message(STATUS "Libs found: ${PHYSX_LIBS}")
message(STATUS "Libs found (shared): ${PHYSX_LIBS_SHARED}")

target_include_directories(phys PUBLIC ${PHYSX_INC_DIR})
target_include_directories(phys PUBLIC ${PHYSX_INC_DIR_SHARED})
target_include_directories(phys PUBLIC -Ilibs/glm -Ilibs/glm/glm)

target_link_libraries(phys PUBLIC ${PHYSX_LIBS})
target_link_libraries(phys PUBLIC ${PHYSX_LIBS_SHARED})
target_link_libraries(phys PUBLIC -Llibs/glm/build/ -lglm_static)

#target_link_libraries(phys PUBLIC -undefined dynamic_lookup)