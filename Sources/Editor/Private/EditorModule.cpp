﻿
#include "ModuleManager.h"
#include "OpenGLRenderer.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "RenderApi.h"
#include "OpenGLRenderApi.h"
#include "IO/Log.h"


const char* Default_VS = "#version 330 core \n\
layout(location = 0) in vec3 aPos; \
layout(location = 1) in vec2 aTexCoords; \
layout(location = 2) in vec3 aNormal; \
layout(location = 3) in vec3 aTangent; \
 \
out vec3 FragPos; \
out vec3 Normal; \
out vec2 TexCoords; \
 \
uniform mat4 model; \
uniform mat4 view; \
uniform mat4 projection; \
 \
void main() \
{ \
	Normal = mat3(transpose(inverse(model))) * aNormal; \
	TexCoords = aTexCoords; \
	FragPos = vec3(model * vec4(aPos, 1.0)); \
	gl_Position = vec4(aPos, 1.0); \
}";

const char* Default_Fs = "#version 330 core \n\
out vec4 FragColor; \
 \
in vec3 FragPos; \
in vec3 Normal; \
in vec2 TexCoords; \
 \
void main() \
{ \
	FragColor = vec4(TexCoords.xy,0,1); \
}";

std::vector<SMeshData::SVertice> Default_Vertices{
	SMeshData::SVertice(SVector(-.9f, -.9f, 0), SVector2D(0,0)),
	SMeshData::SVertice(SVector(.9f, -.9f, 0), SVector2D(1,0)),
	SMeshData::SVertice(SVector(.9f, .9f, 0), SVector2D(1,1)),
	SMeshData::SVertice(SVector(-.9f, .9f, 0), SVector2D(0,1)),
};
std::vector<uint32_t> Default_Triangles{
	0, 1, 2,
	0, 2, 3
};



MODULE_CONSTRUCTOR() {

// 	// Load AssetManager Module (load assets)
// 	AssetManager::Get()->LoadAssetLibrary("");
// 	AssetManager::Get()->GetAsset()

	SRendererApi::Create<SOpenGlRenderApi>();
	SRenderer* EditorRenderer = new SOpenGLRenderer();

	SMaterial BasicMaterial = SMaterial(Default_VS, Default_Fs);
	// Create some components	
	SMeshData BasicMeshData = SMeshData();
	BasicMeshData.Mesh = Default_Vertices;
	BasicMeshData.Triangles = Default_Triangles;
	BasicMeshData.Material = &BasicMaterial;
	IMesh* BasicMesh = new SStaticMesh(BasicMeshData);
	new SMeshComponent(EditorRenderer, BasicMesh, {});


	// Temp render loop
	while (true) {
		SRendererApi::Get()->BeginFrame();
		EditorRenderer->DrawFrame();
		SRendererApi::Get()->EndFrame();
	}
}