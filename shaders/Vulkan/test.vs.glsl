#version 450

layout(set = 0, binding = 0, std140) uniform type_WorldData
{
    layout(row_major) mat4 projection;
    layout(row_major) mat4 view;
} WorldData;

layout(set = 0, binding = 1, std140) uniform type_ObjectData
{
    layout(row_major) mat4 model;
} ObjectData;

layout(location = 0) in vec3 in_var_SV_POSITION;
layout(location = 1) in vec3 in_var_NORMAL;
layout(location = 2) in vec2 in_var_COLOR;
layout(location = 3) in vec2 in_var_TEXCOORD0;
layout(location = 4) in vec3 in_var_TANGENT;
layout(location = 0) out vec2 out_var_TEXCOORD0;

void main()
{
    vec4 _46 = WorldData.projection * (WorldData.view * (ObjectData.model * vec4(in_var_SV_POSITION, 1.0)));
    vec4 _49 = _46;
    _49.y = -_46.y;
    gl_Position = _49;
    out_var_TEXCOORD0 = in_var_TEXCOORD0;
}

