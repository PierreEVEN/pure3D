#Stb
create_virtual_module(Stb "" "./stb/")
#Glfw
if (WIN32)
	create_virtual_module(Glfw "${INTERMEDIATES_DIR}/Dependencies/glfw3" "./glfw/include")
else()
	create_virtual_module(Glfw "${INTERMEDIATES_DIR}/Dependencies/libglfw3" "./glfw/include")
endif()

#Gl3w
add_library(Gl3w STATIC gl3w/src/gl3w.c)
target_include_directories(Gl3w PUBLIC gl3w/include)
if (UNIX)
	target_compile_options(Gl3w PUBLIC -fPIC)
endif()
set_target_properties(Gl3w PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${BINARIES_DIR}/Lib"
	RUNTIME_OUTPUT_DIRECTORY "${BINARIES_DIR}/Bin"
	VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT}")
set_target_properties(Gl3w PROPERTIES FOLDER ThirdParty)