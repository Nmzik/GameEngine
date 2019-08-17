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
    uchar4 tx [[attribute(2)]];
    float2 test [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
};

struct Uniforms {
    float4x4 projViewMatrix;
    float4x4 modelMatrix;
};

vertex VertexOut basic_vertex(const device VertexIn* vertex_array   [[buffer(0)]],
                              const device Uniforms& uniforms       [[buffer(1)]],
                              unsigned int vid [[vertex_id]]) {
    VertexOut vertexOut;
    vertexOut.position = uniforms.projViewMatrix * uniforms.modelMatrix * float4(vertex_array[vid].position, 1);
    
    return vertexOut;
}

fragment half4 basic_fragment(VertexOut interpolated [[stage_in]]) {
    return half4(0.5, 0.5, 0.5, 0.5);
}
