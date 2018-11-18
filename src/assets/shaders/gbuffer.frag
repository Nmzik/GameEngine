#version 430 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gNormal;

layout (location = 0) in vec2 TexCoords;
layout (location = 1) in vec3 Normal;
layout (location = 2) in mat3 TBN;

layout(binding = 0) uniform sampler2D DiffuseSampler;
layout(binding = 1) uniform sampler2D BumpSampler;
layout(binding = 2) uniform sampler2D SpecSampler;

uniform int useBump;

void main()
{   
	gAlbedoSpec = texture(DiffuseSampler, TexCoords);
	if (gAlbedoSpec.a < 0.75)
		discard;
	/*if (useBump) {
		vec3 normal = texture(BumpSampler, TexCoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
		gNormal = normalize(TBN * normal);
	} else {*/
		gNormal = normalize(Normal);
	//}
	gAlbedoSpec.a = texture(SpecSampler, TexCoords).r;
}