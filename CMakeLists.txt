cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PureHeaderTests)

project(pure3D)

# Pure3D only support x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "32-bits is not supported")
endif()

# Options
option(PHT_DEBUG "Enable debug mode for PureReflectionTool" OFF)


# Set compiler options
set(CMAKE_CXX_STANDARD 17)
if (MSVC)
	set(CMAKE_CXX_FLAGS "/MP /EHsc")
endif()
	
	message(${CMAKE_CXX_FLAGS})

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

function(use_reflection)

	add_private_dependencies(PureHeaderLib)	
	
	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	message ("-- -- => Add reflection data for " ${TARGET})
	set(REFLECTION_TARGET ${TARGET}_ReflectionGenerator)
	
	add_custom_target(${REFLECTION_TARGET} ALL
					COMMAND "$<TARGET_FILE:PureHeaderGenerator>" -ModuleName="${TARGET}" -ModulePath="${CMAKE_CURRENT_SOURCE_DIR}" -Debug=${PHT_DEBUG} -OutputPath=${REFLECTION_DIR}/${TARGET})
					
	set_target_properties(${REFLECTION_TARGET} PROPERTIES FOLDER "ReflectionData")
	
	add_dependencies(${REFLECTION_TARGET} PureHeaderGenerator)
	add_dependencies(${TARGET} ${REFLECTION_TARGET})
	
	target_include_directories(${TARGET}
		PRIVATE
			${REFLECTION_DIR}/${TARGET}/Public)
			
	file(GLOB_RECURSE SOURCES ${REFLECTION_DIR}/${TARGET}/Private/*.refl.cpp)
		
	target_sources(${TARGET} PRIVATE ${SOURCES})
		
endfunction()

# Set current project target catogory
function(set_project_category CATEGORY)
	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	set_target_properties(${TARGET} PROPERTIES FOLDER ${CATEGORY})
endfunction()

# Create a lib module
function(create_library)

	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

	message("-- => Building module " ${TARGET})

	file(GLOB_RECURSE SOURCES *.cpp *.hpp *.h *.c)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
	add_library(${TARGET} ${SOURCES} ${ARGN})

	module_definition(${TARGET})
	
endfunction()

# Create an executable module
function(create_executable)

	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

	message("-- => Building module " ${TARGET})

	file(GLOB_RECURSE SOURCES *.cpp *.hpp *.h *.c)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
	add_executable(${TARGET} ${SOURCES} ${ARGN})

	module_definition(${TARGET})
	
endfunction()

#link public dependencies
function(add_public_dependencies)
	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

	message("-- -- => Linking public modules {" ${ARNG} "}")

	target_include_directories(${TARGET} PUBLIC ${ARGN})
	target_link_libraries(${TARGET} PUBLIC ${ARGN})
endfunction()

#link private dependencies
function(add_private_dependencies)
	get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

	message("-- -- => Linking private modules {" ${ARNG} "}")

	target_include_directories(${TARGET} PRIVATE ${ARGN})
	target_link_libraries(${TARGET} PRIVATE ${ARGN})
endfunction()

add_subdirectory(Sources)
