#include "OpenGLRenderApi.h"
#include "IO/Log.h"
#include "Mesh.h"

struct SOpenGLShaderHandle : SShaderHandle {
	SOpenGLShaderHandle(GLuint inHandle) : ShaderHandle(inHandle) {}
	GLuint ShaderHandle;
};

struct SOpenGLTexture2DHandle : STextureHandle {
	SOpenGLTexture2DHandle(GLuint inHandle) : ShaderHandle(inHandle) {}
	GLuint ShaderHandle;
};

struct SOpenGLMeshHandle : SMeshHandle {
	SOpenGLMeshHandle(GLuint inVbo, GLuint inEbo, GLuint inVao, uint32_t inVertices, uint32_t inTriangles) : Vbo(inVbo), Ebo(inEbo), Vao(inVao), Vertices(inVertices), Triangles(inTriangles) {}
	GLuint Vbo, Ebo, Vao;
	uint32_t Vertices, Triangles;
};

void checkCompileErrors(unsigned int shader, const char* type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("Failed to compile shader %s : %s", type, infoLog);
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("Failed to compile shader %s : %s", type, infoLog);
		}
	}
}

SOpenGlRenderApi::SOpenGlRenderApi() {
	StartOpenGL();
}

void SOpenGlRenderApi::StartOpenGL() {
	LOG("Create Glfw context");
	if (!glfwInit()) LOG_ASSERT("Failed to create glfw window");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

	WindowHandle = glfwCreateWindow(800, 600, "test", 0, 0);
	if (!WindowHandle) LOG_ASSERT("Failed to create Glfw window handle");
	glfwMakeContextCurrent(WindowHandle);

	LOG("Initialize OpenGL context");
	if (gl3wInit()) {
		LOG_ASSERT("failed to initialize OpenGL");
	}
	if (!gl3wIsSupported(3, 2)) {
		LOG_ASSERT("OpenGL 3.2 not supported");
	}
	LOG("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void SOpenGlRenderApi::DrawMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy) {
	SMeshProxy* Meshproxy = static_cast<SMeshProxy*>(Proxy);
	std::shared_ptr<SOpenGLMeshHandle> MeshHandle = dynamic_pointer_cast<SOpenGLMeshHandle>(Meshproxy->MeshHandle);
	//Meshproxy->Transform; // Material->setMat4

	if (!MeshHandle) return;

	glBindVertexArray(MeshHandle->Vbo);
	if (MeshHandle->Triangles > 0)
		glDrawElements(GL_TRIANGLES, MeshHandle->Triangles, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, MeshHandle->Vertices);
}

void SOpenGlRenderApi::BeginFrame() {
	glfwPollEvents();
}

void SOpenGlRenderApi::EndFrame() {
	glfwSwapBuffers(WindowHandle);
}

std::shared_ptr<SShaderHandle> SOpenGlRenderApi::CompileShader(const String& VertexShader, const String& FragmentShader) {
	const char* vShaderCode = VertexShader.GetData();
	const char* fShaderCode = FragmentShader.GetData();

	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	GLuint ShaderID = glCreateProgram();
	glAttachShader(ShaderID, vertex);
	glAttachShader(ShaderID, fragment);
	glLinkProgram(ShaderID);
	checkCompileErrors(ShaderID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return std::make_shared<SOpenGLShaderHandle>(ShaderID);
}

std::shared_ptr<STextureHandle> SOpenGlRenderApi::CreateTexture(const uint8_t* TextureData, uint32_t Width, uint32_t Height, uint32_t Channels) {
	GLuint TextureID;

	GLenum format = GL_RED;
	if (Channels == 1)
		format = GL_RED;
	else if (Channels == 3)
		format = GL_RGB;
	else if (Channels == 4)
		format = GL_RGBA;

	glGenTextures(1, &TextureID);
	//glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, TextureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//stbi_image_free(data);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	return std::make_shared<SOpenGLTexture2DHandle>(TextureID);
}

std::shared_ptr<SMeshHandle> SOpenGlRenderApi::CreateMesh(const SMeshData* Data) {

	GLuint Vbo, Ebo, Vao;

	// create buffers/arrays
	glGenVertexArrays(1, &Vao);
	glGenBuffers(1, &Vbo);
	glGenBuffers(1, &Ebo);

	glBindVertexArray(Vao);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.

	glBufferData(GL_ARRAY_BUFFER, Data->Mesh.size() * sizeof(SMeshData::SVertice), &Data->Mesh[0], GL_STATIC_DRAW);

	if (Data->Triangles.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Data->Triangles.size() * sizeof(unsigned int), &Data->Triangles[0], GL_STATIC_DRAW);
	}

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SMeshData::SVertice), (void*)offsetof(SMeshData::SVertice, Position));
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SMeshData::SVertice), (void*)offsetof(SMeshData::SVertice, UV));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SMeshData::SVertice), (void*)offsetof(SMeshData::SVertice, Normal));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(SMeshData::SVertice), (void*)offsetof(SMeshData::SVertice, Color));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SMeshData::SVertice), (void*)offsetof(SMeshData::SVertice, Tangent));

	glBindVertexArray(0);
	return std::make_shared<SOpenGLMeshHandle>(Vbo, Ebo, Vao, static_cast<uint32_t>(Data->Mesh.size()), static_cast<uint32_t>(Data->Triangles.size()));
}
