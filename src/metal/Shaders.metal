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
    float3 fragmentPosition;
    float3 normal;
    float2 texCoord;
};

struct Uniforms {
    float4x4 projMatrix;
    float4x4 ViewMatrix;
    float4x4 modelMatrix;
};

vertex VertexOut basic_vertex(const VertexIn vertex_array   [[stage_in]],
                              const device Uniforms& uniforms       [[buffer(1)]],
                              unsigned int vid [[vertex_id]]) {
    VertexOut vertexOut;
    
    vertexOut.position = uniforms.projMatrix * uniforms.ViewMatrix * uniforms.modelMatrix * float4(vertex_array.position, 1);
    vertexOut.fragmentPosition = (uniforms.modelMatrix * float4(vertex_array.position, 1.0)).xyz;
    //vertexOut.normal = (uniforms.ViewMatrix * uniforms.modelMatrix * float4(vertex_array.normal, 0.0)).xyz;
    vertexOut.normal = vertex_array.normal;
    vertexOut.texCoord = vertex_array.texCoord;
    
    return vertexOut;
}

/*fragment float4 basic_fragment(VertexOut out [[stage_in]],
                              texture2d<float> tex2d [[texture(0)]],
                              sampler sampler2d [[sampler(0)]]) {
    float4 texSample = float4(tex2d.sample(sampler2d, out.texCoord));
    
    float ambientIntensity = 0.5;
    float3 lightColor = {1.0, 1.0, 1.0};
    float3 direction = {0.0, 0.0, 1.0};
    float diffuseIntensity = 1;
    
    float4 ambientColor = float4(lightColor * ambientIntensity, 1.0);
    
    float3 norm = normalize(out.normal);
    float diffuseFactor = max(0.0, dot(norm, direction));
    float4 diffuseColor = float4(lightColor * diffuseIntensity * diffuseFactor, 1.0);
    
    float shininess = 10;
    float specularIntensity = 2;
    float3 eye = normalize(out.fragmentPosition);
    float3 reflection = reflect(direction, out.normal);
    float specularFactor = pow(max(0.0, dot(reflection, eye)), shininess);
    float4 specularColor = float4(lightColor * specularIntensity * specularFactor, 1.0);
    //texSample.a = de1.0;
    //discard_fragment();
    //return texSample;
    return texSample * (ambientColor + diffuseColor + specularColor);
}*/

fragment half4 basic_fragment(VertexOut out [[stage_in]],
                               texture2d<float> tex2d [[texture(0)]],
                               sampler sampler2d [[sampler(0)]]) {
    half4 texSample = half4(tex2d.sample(sampler2d, out.texCoord));
    
    //if (texSample.a <= 0.5)
      //  discard_fragment();
    
    half ambientIntensity = 0.5;
    half3 lightColor = {1.0, 1.0, 1.0};
    float3 direction = {0.0, 0.0, 1.0};
    half diffuseIntensity = 1;
    
    half4 ambientColor = half4(lightColor * ambientIntensity, 1.0);
    
    float3 norm = normalize(out.normal);
    half diffuseFactor = max(0.0, dot(norm, direction));
    half4 diffuseColor = half4(lightColor * diffuseIntensity * diffuseFactor, 1.0);
    
    float shininess = 10;
    float specularIntensity = 2;
    float3 eye = normalize(out.fragmentPosition);
    float3 reflection = reflect(direction, out.normal);
    float specularFactor = pow(max(0.0, dot(reflection, eye)), shininess);
    half4 specularColor = half4(lightColor * specularIntensity * specularFactor, 1.0);
    return texSample * (ambientColor + diffuseColor + specularColor);
}
