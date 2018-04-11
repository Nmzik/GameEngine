#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 _color;
layout (location = 3) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec4 Color;
//out float flogz;

layout(location = 3) uniform mat4 model;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 projection;

//const float Fcoef = 2.0 / log2(10000 + 1.0);

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * aNormal;
	//Normal = mat3(transpose(inverse(view * model))) * aNormal;
	Color = _color;

    gl_Position = projection * viewPos;
	//gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
	//flogz = 1.0 + gl_Position.w;
}

#shader fragment
#version 430 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;
//in float flogz;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

//const float Fcoef = 2.0 / log2(10000 + 1.0);

void main()
{    
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
	/*vec4 texColor = texture(material.diffuse, TexCoords);
	if (texColor.a < 0.1)
		discard;*/
	//gAlbedoSpec = vec4(Color.b, Color.g, Color.r, Color.a);
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a *= texture(material.specular, TexCoords).r;
	//gl_FragDepth = log2(flogz) * (Fcoef / 2.0);
}