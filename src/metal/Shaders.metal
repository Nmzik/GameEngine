//
//  Shaders.metal
//  gameMac
//
//  Created by nmzik on 12.08.2019.
//  Copyright © 2019 nmzik. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 normal [[attribute(1)]];
    uint colour [[attribute(2)]];
    float2 texCoord [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoord;
};

struct Uniforms {
    float4x4 projViewMatrix;
    float4x4 modelMatrix;
};

vertex VertexOut basic_vertex(const VertexIn vertex_array   [[stage_in]],
                              const device Uniforms& uniforms       [[buffer(1)]],
                              unsigned int vid [[vertex_id]]) {
    VertexOut vertexOut;
    
    vertexOut.position = uniforms.projViewMatrix * uniforms.modelMatrix * float4(vertex_array.position, 1);
    vertexOut.texCoord = vertex_array.texCoord;
    
    return vertexOut;
}

fragment half4 basic_fragment(VertexOut out [[stage_in]],
                              texture2d<float> tex2d [[texture(0)]],
                              sampler sampler2d [[sampler(0)]]) {
    half4 texSample = half4(tex2d.sample(sampler2d, out.texCoord));
    //texSample.a = 1.0;
        //discard_fragment();
    //return texSample;
    return texSample;
}
