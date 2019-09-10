//
//  guiShader.metal
//  MyGameEngine
//
//  Created by nmzik on 30.08.2019.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    packed_float2 vertices;
    packed_float2 uvs;
};

struct VertexOut {
    float4 position [[position]];
    float2 uv;
};

struct Uniforms {
    float2 alignment;
};

vertex VertexOut vertex_gui(const device VertexIn* vertices,
                            const device Uniforms& uniforms [[buffer(1)]],
                            unsigned int vid [[vertex_id]])
{
    float4x4 proj = float4x4(1.0);
    float2 pos = vertices[vid].vertices;
    //1024 576
    //pos /= float2(40, 40);
    //pos -=
    VertexOut vertexOut;
    vertexOut.position = proj * float4(pos + uniforms.alignment, 0.0, 1.0);
    vertexOut.uv = vertices[vid].uvs;
    return vertexOut;
}

fragment float4 fragment_gui(VertexOut out [[stage_in]],
                             texture2d<float> tex2d [[texture(0)]],
                             sampler sampler2d [[sampler(0)]])
{
    float4 tex = tex2d.sample(sampler2d, out.uv);
    return tex;
}
