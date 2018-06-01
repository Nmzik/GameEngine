#shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;

layout(location = 3) uniform mat4 model;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 projection;

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * aNormal;
    
    gl_Position = projection * viewPos;
}

#shader fragment
#version 430 core
layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D normalTex;
uniform sampler2D dudvMap;
uniform float waveSpeed;
uniform float waveStrength;
uniform float sumTimeStep;
uniform vec3 waterColor;

void main()
{    
	float speed = sumTimeStep * waveSpeed;
	vec2 distortedTexCoords = texture(dudvMap, vec2(TexCoords.x + speed, TexCoords.y)).rg * 0.1;
	distortedTexCoords = textCoordinates + vec2(distortedTexCoords.x, distortedTexCoords.y * speed);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

	vec2 normalTextCoordinates = TexCoords + totalDistortion;
	vec4 normal = texture2D(normalTex, normalTextCoordinates);
	gNormal = vec3(normal.xyz);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = waterColor;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 0.5;
}