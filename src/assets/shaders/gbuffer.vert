#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 tangent;

layout (location = 0) out vec2 TexCoords;
layout (location = 1) out vec3 Normal;
layout (location = 2) out mat3 TBN;

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