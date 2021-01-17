
#include "ModuleManager.h"
#include "OpenGLRenderer.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "RenderApi.h"
#include "IO/Log.h"
#include "GlfwInputs.h"
#include "OpenGLRenderApi.h"
#include "AssetFactory.h"


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
 \
layout(std140) uniform shader_data \
{ \
	mat4 viewMatrix; \
	mat4 projectionMatrix; \
	vec4 cameraPosition; \
	vec4 cameraDirection; \
	vec2 frameBufferResolution; \
	float Time; \
}; \
 \
void main() \
{ \
	Normal = mat3(transpose(inverse(model))) * aNormal; \
	TexCoords = aTexCoords; \
	FragPos = vec3(model * vec4(aPos, 1.0)); \
 \
	vec4 pos = vec4(aPos, 1.0) + vec4(5, 0, -1, 0); \
	gl_Position = projectionMatrix * viewMatrix * pos; \
}";

const char* Default_Fs = "#version 330 core \n\
out vec4 FragColor; \
 \
in vec3 FragPos; \
in vec3 Normal; \
in vec2 TexCoords; \
 \
 \
layout(std140) uniform shader_data \
{ \
	mat4 viewMatrix; \
	mat4 projectionMatrix; \
	vec4 cameraPosition; \
	vec4 cameraDirection; \
	vec2 frameBufferResolution; \
	float Time; \
}; \
 \
void main() \
{ \
	FragColor = vec4(TexCoords.xy + sin(Time), 0, 1); \
}";

std::vector<SMeshData::SVertice> DEFAULT_VERTICES{
	SMeshData::SVertice(SVector(-.9f, -.9f, 0), SVector2D(0,0)),
	SMeshData::SVertice(SVector(.9f, -.9f, 0), SVector2D(1,0)),
	SMeshData::SVertice(SVector(.9f, .9f, .5f), SVector2D(1,1)),
	SMeshData::SVertice(SVector(-.9f, .9f, 0), SVector2D(0,1)),
};
std::vector<uint32_t> DEFAULT_TRIANGLES{
	0, 1, 2,
	0, 2, 3
};

class SCameraController : public IInputListener {
public:
	SCameraController(SRenderer* InRenderer) : Renderer(InRenderer) {}

	SRenderer* Renderer;
	SVectorDouble Input;

	double LastPosX = 0, LastPosY = 0;
	bool bFirstSet = true;
	double Pitch = 0, Yaw = 0;

	void KeyCallback(int keycode, int ScanCode, int action, int mods) override {
		if (action == GLFW_PRESS) {
			switch (keycode) {
			case GLFW_KEY_W:
				Input.x += 1;
				break;
			case GLFW_KEY_D:
				Input.y += 1;
				break;
			case GLFW_KEY_SPACE:
				Input.z -= 1;
				break;
			case GLFW_KEY_S:
				Input.x -= 1;
				break;
			case GLFW_KEY_A:
				Input.y -= 1;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				Input.z += 1;
				break;
				default:
					break;
			}
		}
		if (action == GLFW_RELEASE) {
			switch (keycode) {
			case GLFW_KEY_W:
				Input.x -= 1;
				break;
			case GLFW_KEY_D:
				Input.y -= 1;
				break;
			case GLFW_KEY_SPACE:
				Input.z += 1;
				break;
			case GLFW_KEY_S:
				Input.x += 1;
				break;
			case GLFW_KEY_A:
				Input.y += 1;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				Input.z -= 1;
				break;
			default:
				break;
			}
		}
	}

	void Update() override {
		Renderer->GetCamera().GetCameraTransform().SetLocation(Renderer->GetCamera().GetCameraTransform().GetLocation() + Renderer->GetCamera().GetCameraTransform().GetRotation().GetForwardVector() * Input.x);
		Renderer->GetCamera().GetCameraTransform().SetLocation(Renderer->GetCamera().GetCameraTransform().GetLocation() + Renderer->GetCamera().GetCameraTransform().GetRotation().GetRightVector() * Input.y);
		Renderer->GetCamera().GetCameraTransform().SetLocation(Renderer->GetCamera().GetCameraTransform().GetLocation() + Renderer->GetCamera().GetCameraTransform().GetRotation().GetUpVector() * Input.z);
	}
	void CharCallback(int chr) override {}
	void MouseButtonCallback(int button, int action, int mods) override {}
	void ScrollCallback(double xOffset, double yOffset) override {
		Renderer->GetCamera().SetFieldOfView(Renderer->GetCamera().GetFieldOfView() - static_cast<float>(yOffset) * 0.02f);
	}

	void CursorPosCallback(double X, double Y) override {
		double DeltaX = 0;
		double DeltaY = 0;
		if (bFirstSet) {
			bFirstSet = false;
		}
		else {
			DeltaX = X - LastPosX;
			DeltaY = Y - LastPosY;
		}
		LastPosX = X;
		LastPosY = Y;

		Yaw -= DeltaX / 5;
		Pitch -= DeltaY / 5;

		if (Pitch > 80) Pitch = 80;
		if (Pitch < -80) Pitch = -80;

		Renderer->GetCamera().GetCameraTransform().SetRotation(SQuatd(0, Pitch, Yaw));
	}
};


MODULE_CONSTRUCTOR() {
	// Load dependencies
	ModuleManager::LoadModule("Mesh");
	ModuleManager::LoadModule("Shaders");
	
	IRendererApi::Create<SOpenGlRenderApi>();
	SRenderer* EditorRenderer = new SOpenGLRenderer();
	IInputManager::Create<SGlfwInputManager>();
	SCameraController Controller(EditorRenderer);

	
	SMaterial* BasicMaterial = SAssetFactory::CreateFromData<SMaterial, const String&, const String&>(Default_VS, Default_Fs);
	if (!BasicMaterial) LOG_ASSERT("failed to create material");
	// Create some components	
	SMeshData BasicMeshData = SMeshData();
	BasicMeshData.Mesh = DEFAULT_VERTICES;
	BasicMeshData.Triangles = DEFAULT_TRIANGLES;
	BasicMeshData.Material = BasicMaterial;
	IMesh* BasicMesh = SAssetFactory::CreateFromData<SStaticMesh, const std::vector<SMeshData::SVertice>&, const std::vector<uint32_t>&, const std::vector<SMaterial*>&>(DEFAULT_VERTICES, DEFAULT_TRIANGLES, {});
	if (!BasicMesh) LOG_ASSERT("failed to create mesh");
	new SMeshComponent(EditorRenderer, BasicMesh, {});

	EditorRenderer->GetCamera().SetFieldOfView(120);
	
	// Temp render loop
	while (!IRendererApi::Get()->ShouldCloseWindow()) {
		IInputManager::Get()->Update();
		IRendererApi::Get()->BeginFrame();
		EditorRenderer->DrawFrame();
		IRendererApi::Get()->EndFrame();
	}
}