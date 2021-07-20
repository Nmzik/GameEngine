#version 450

layout(set = 0, binding = 1) uniform texture2D diffuseMap;
layout(set = 0, binding = 0) uniform sampler sampleWrap;

layout(location = 0) in vec2 in_var_TEXCOORD0;
layout(location = 0) out vec4 out_var_SV_TARGET;

void main()
{
    out_var_SV_TARGET = texture(sampler2D(diffuseMap, sampleWrap), in_var_TEXCOORD0);
}

