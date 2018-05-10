#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 tangent;

out vec2 TexCoords;
out vec3 Normal;
out mat3 TBN;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = cross(normalize(Normal), tangent);
	vec3 N = normalize(Normal);

	TBN = transpose(mat3(T, B, N));

    gl_Position = projection * viewPos;
}

#shader fragment
#version 430 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gNormal;

in vec2 TexCoords;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D DiffuseSampler;
uniform sampler2D BumpSampler;
uniform sampler2D SpecSampler;

void main()
{    
	vec3 normal = texture(BumpSampler, TexCoords).rgb;
	// transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	gNormal = normalize(normal);
    // and the diffuse per-fragment color
	/*vec4 texColor = texture(material.diffuse, TexCoords);
	if (texColor.a < 0.1)
		discard;*/
	//gAlbedoSpec = vec4(Color.b, Color.g, Color.r, Color.a);
    gAlbedoSpec.rgb = texture(DiffuseSampler, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
	gAlbedoSpec.a = texture(SpecSampler, TexCoords).r;
}