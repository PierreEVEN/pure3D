
#include "ModuleManager.h"
#include "OpenGLRenderer.h"
#include "MeshComponent.h"





MODULE_CONSTRUCTOR() {

	// Load ShaderModule

	// Load StaticMesh Module (initialize Static mesh draw queue / Load ShaderModule too)

	// Load AssetManager Module (load assets)

	// Create renderer

	SOpenGLRenderer::CreateOpenGLContext();

	SRenderer* EditorRenderer = new SOpenGLRenderer();

	// Create some components	
	new MeshComponent(EditorRenderer, new IMesh());

	// Temp render loop
	while (true) {
		EditorRenderer->NextFrame();
	}

	SOpenGLRenderer::CloseOpenGLContext();
}