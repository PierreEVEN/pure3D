#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


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
	FragColor = vec4(TexCoords.xy + sin(Time), 0, 1);
}