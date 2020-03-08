# Project definition
cmake_minimum_required(VERSION 3.1)

#file(GLOB RENDER_SOURCES "^(?!Renderer).*cpp")

file(GLOB RENDER_SOURCES "*cpp") 
	#RELATIVE ".")

message(STATUS "Files captured: ${RENDER_SOURCES}")


add_library(rendering STATIC RENDER_SOURCES)




#[[
foreach (TMPSOURCE RENDER_SOURCES)
	
	string(REGEX REPLACE ".cpp" "" OBJECTNAME ${TMPSOURCE})
	add_library()
]]