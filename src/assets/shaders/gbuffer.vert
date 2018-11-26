#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;

layout (location = 0) out vec2 TexCoords;
layout (location = 1) out vec3 Normal;
layout (location = 2) out mat3 TBN;

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
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

	vec3 T = normalize(vec3(normalMatrix * aTangent));
	vec3 N = normalize(vec3(normalMatrix * aNormal));
	T = normalize(T-dot(T,N)*N);
	vec3 B = cross(N,T); 

	TBN = transpose(mat3(T, B, N));

    gl_Position = projection * viewPos;
}