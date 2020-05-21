#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;

layout (location = 0) out vec2 TexCoords;

layout(binding = 0, std140) uniform WorldData
{
	mat4 projection;
	mat4 view;
};

layout(binding = 1, std140) uniform ObjectData
{
	mat4 model;
};

void main()
{
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}