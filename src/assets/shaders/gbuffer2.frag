#version 430 core
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 TexCoords;
layout (location = 1) in vec3 Normal;
layout (location = 2) in mat3 TBN;

layout(binding = 0) uniform sampler2D DiffuseSampler;
layout(binding = 1) uniform sampler2D BumpSampler;
layout(binding = 2) uniform sampler2D SpecSampler;

uniform int useBump;

void main()
{   
	FragColor = texture(DiffuseSampler, TexCoords);
	if (FragColor.a < 0.75)
		discard;
}