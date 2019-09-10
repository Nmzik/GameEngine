//
//  fxaaShader.metal
//  MyGameEngine
//
//  Created by nmzik on 08.09.2019.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    packed_float2 position;
    packed_float2 texCoord;
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoord;
};

vertex VertexOut fxaa_vertex(const device VertexIn* vertices,
                              unsigned int vid [[vertex_id]]) {
    VertexOut vertexOut;
    
    vertexOut.position = float4(vertices[vid].position, 1);
    vertexOut.texCoord = 1.0 - vertices[vid].texCoord;
    
    return vertexOut;
}

fragment float4 fxaa_fragment(VertexOut out [[stage_in]],
                              texture2d<float> tex2d [[texture(0)]],
                              sampler sampler2d [[sampler(0)]]) {
    float4 texSample = float4(tex2d.sample(sampler2d, 1.0 - out.texCoord));
    
    const float FXAA_SPAN_MAX = 16.0f;
    float FXAA_REDUCE_MUL = 1.0f/8.0f;
    float FXAA_REDUCE_MIN = 1.0f/128.0f;
    float2 hdrBufferOffset = float2(1.0 / 1024, 1.0 / 576);
    
    float3 luma = float3(0.299f, 0.587f, 0.114f);
    
    float3 offsetNW = float3(tex2d.sample(sampler2d, out.texCoord.xy + (float2(-1.0f, -1.0f) * hdrBufferOffset)));
    float3 offsetNE = float3(tex2d.sample(sampler2d, out.texCoord.xy + (float2(1.0f, -1.0f) * hdrBufferOffset)));
    float3 offsetSW = float3(tex2d.sample(sampler2d, out.texCoord.xy + (float2(-1.0f, 1.0f) * hdrBufferOffset)));
    float3 offsetSE = float3(tex2d.sample(sampler2d, out.texCoord.xy + (float2(1.0f, 1.0f) * hdrBufferOffset)));
    float3 offsetM  = float3(tex2d.sample(sampler2d, out.texCoord.xy));
    
    float lumaNW = dot(luma, offsetNW);
    float lumaNE = dot(luma, offsetNE);
    float lumaSW = dot(luma, offsetSW);
    float lumaSE = dot(luma, offsetSE);
    float lumaM  = dot(luma, offsetNW);
    
    float2 dir = float2(-((lumaNW + lumaNE) - (lumaSW + lumaSE)),
                        ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (FXAA_REDUCE_MUL * 0.25f), FXAA_REDUCE_MIN);
    float dirCorrection = 1.0f / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(float2(FXAA_SPAN_MAX), max(float2(-FXAA_SPAN_MAX), dir * dirCorrection)) * hdrBufferOffset;
    
    float3 resultA = 0.5f * float3(tex2d.sample(sampler2d, out.texCoord.xy + (dir * float2(1.0f / 3.0f - 0.5f)))) +
                             float3(tex2d.sample(sampler2d, out.texCoord.xy + (dir * float2(2.0f / 3.0f - 0.5f))));
    
    float3 resultB = resultA * 0.5f + 0.25f * float3(tex2d.sample(sampler2d, out.texCoord.xy + (dir * float2(0.0f / 3.0f - 0.5f)))) +
                                               float3(tex2d.sample(sampler2d, out.texCoord.xy + (dir * float2(3.0f / 3.0f - 0.5f))));
    
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    float lumaResultB = dot(luma, resultB);
    
    if(lumaResultB < lumaMin || lumaResultB > lumaMax)
        return float4(resultA, 1.0);
    else
        return float4(resultB, 1.0);
    
    
    
    return texSample;
}
