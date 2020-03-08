# Project definition
cmake_minimum_required(VERSION 3.1)

add_library()

message(STATUS "Files captured: ${RENDER_SOURCES}")




#[[
foreach (TMPSOURCE RENDER_SOURCES)
	
	string(REGEX REPLACE ".cpp" "" OBJECTNAME ${TMPSOURCE})
	add_library()
]]