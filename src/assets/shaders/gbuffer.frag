#version 430 core
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 TexCoords;

layout(binding = 0) uniform sampler2D DiffuseSampler;

uniform int useBump;

void main()
{   
	FragColor = texture(DiffuseSampler, TexCoords);
}