#ImGui
file(GLOB SOURCES imgui/*.cpp imgui/*.h)
create_custom_target_library(ImGui ${SOURCES})
#ImGuizmo
file(GLOB SOURCES ImGuizmo/*.cpp ImGuizmo/*.h)
create_custom_target_library(ImGuizmo ${SOURCES})
target_include_directories(ImGuizmo PRIVATE "imgui")
target_link_libraries(ImGuizmo PRIVATE "ImGui")
#Stb
file(GLOB SOURCES stb/stb_truetype.h stb/stb_image.h stb.cpp)
create_custom_target_library(Stb ${SOURCES})
	target_include_directories(Stb
		PUBLIC
			stb)