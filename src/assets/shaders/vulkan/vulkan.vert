#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(binding = 0) uniform WorldData
{
	mat4 projection;
	mat4 view;
};

layout(push_constant) uniform ObjectData
{
	mat4 model;
};

void main()
{
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}