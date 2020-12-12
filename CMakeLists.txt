cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PureHeaderLib)
set(CMAKE_SUPPRESS_REGENERATION true)

project(pure3D)

# Pure3D only support x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "32-bits is not supported")
endif()

# Set compiler options
set(CMAKE_CXX_STANDARD 17)

# Set project constants
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(BINARIES_DIR ${PROJECT_ROOT}/Binaries)
set(INTERMEDIATES_DIR ${PROJECT_ROOT}/Intermediates)
set(REFLECTION_DIR ${PROJECT_ROOT}/Intermediates/Reflection)

# Basic module definition
function(module_definition TARGET)

	set_target_properties(${TARGET} PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${BINARIES_DIR}/${TARGET}"
		LIBRARY_OUTPUT_DIRECTORY "${BINARIES_DIR}/${TARGET}"
		RUNTIME_OUTPUT_DIRECTORY "${BINARIES_DIR}/${TARGET}"
		VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT}")

	target_include_directories(${TARGET}
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/Public	
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/Private)

endfunction()

# Create an executable module
function(create_executable)

	message("-- => Building module " ${TARGET})

	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

	file(GLOB_RECURSE SOURCES *.cpp *.hpp *.h *.c)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
	add_executable(${TARGET} ${SOURCES} ${ARGN})

	module_definition(${TARGET})
	
endfunction()

add_subdirectory(Sources)