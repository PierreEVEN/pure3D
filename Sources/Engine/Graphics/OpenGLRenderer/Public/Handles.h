#pragma once
#include "RenderApi.h"
#include "GL/glcorearb.h"

struct SOpenGLShaderHandle : SShaderHandle {
	SOpenGLShaderHandle(GLuint inHandle) : ShaderHandle(inHandle) {}
	GLuint ShaderHandle;
};

/*
struct SOpenGLTexture2DHandle : STextureHandle {
	SOpenGLTexture2DHandle(GLuint inHandle) : ShaderHandle(inHandle) {}
	GLuint ShaderHandle;
};
*/

struct SOpenGLMeshHandle : SMeshHandle {
	SOpenGLMeshHandle(GLuint inVbo, GLuint inEbo, GLuint inVao, uint32_t inVertices, uint32_t inTriangles) : Vbo(inVbo), Ebo(inEbo), Vao(inVao), Vertices(inVertices), Triangles(inTriangles) {}
	GLuint Vbo, Ebo, Vao;
	uint32_t Vertices, Triangles;
};
