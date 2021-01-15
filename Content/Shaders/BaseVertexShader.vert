version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


layout(std140) uniform shader_data
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vec4 cameraPosition;
	vec4 cameraDirection;
	vec2 framebufferResolution;
	float Time;
};

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPos, 1.0));

	vec4 pos = vec4(aPos, 1.0) + vec4(5, 0, -1, 0);
	gl_Position = projectionMatrix * viewMatrix * pos;
}