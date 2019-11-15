//
//  Shaders.metal
//  gameMac
//
//  Created by nmzik on 12.08.2019.
//  Copyright © 2019 nmzik. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct VertexInPCT {
    float3 position [[attribute(0)]];
    float4 colour [[attribute(1)]];
    float2 texCoord [[attribute(2)]];
};

struct VertexInDefault {
    float3 position [[attribute(0)]];
    float3 normal [[attribute(1)]];
    uchar4 colour [[attribute(2)]];
    float2 texCoord [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 fragmentPosition;
    float3 normal;
    uchar4 color;
    float2 texCoord;
    float4 tangent;
    float4 Bitangent;
};

struct Uniforms {
    float4x4 projMatrix;
    float4x4 ViewMatrix;
    float4x4 modelMatrix;
};

float3 NormalMap(float2 nmv, float bumpinezz, float3 norm, float3 tang, float3 bita)
{
    //r1 = nmv; //sample r1.xyzw, v2.xyxx, t2.xyzw, s2  (BumpSampler)
    //float bmp = max(bumpinezz, 0.001);   //max r0.x, bumpiness, l(0.001000)
    float2 nxy = nmv.xy * 2 - 1;          //mad r0.yz, r1.xxyx, l(0.000000, 2.000000, 2.000000, 0.000000), l(0.000000, -1.000000, -1.000000, 0.000000)
    float2 bxy = nxy * max(bumpinezz, 0.001);          //mul r0.xw, r0.xxxx, r0.yyyz
    float bxyz = sqrt(abs(1 - dot(nxy, nxy)));    //r0.y = dot(nxy, nxy);       //dp2 r0.y, r0.yzyy, r0.yzyy  //r0.y = 1.0 - r0.y;              //add r0.y, -r0.y, l(1.000000)  //r0.y = sqrt(abs(r0.y));         //sqrt r0.y, |r0.y|
    float3 t1 = tang * bxy.x; //mad r0.xzw, r0.xxxx, v4.xxyz, r1.xxyz
    float3 t2 = bita * bxy.y + t1;    //mul r1.xyz, r0.wwww, v5.xyzx
    float3 t3 = norm * bxyz + t2; //mad r0.xyz, r0.yyyy, v3.xyzx, r0.xzwx
    return normalize(t3);
    //r0.w = dot(t3, t3);     //dp3 r0.w, r0.xyzx, r0.xyzx
    //r0.w = 1.0 / sqrt(r0.w);        //rsq r0.w, r0.w
    ////r1.x = r0.z*r0.w - 0.35;        //mad r1.x, r0.z, r0.w, l(-0.350000)
    //t3 = t3*r0.w;           //mul r0.xyz, r0.wwww, r0.xyzx
    ////mad o1.xyz, t3.xyzx, l(0.500000, 0.500000, 0.500000, 0.000000), l(0.500000, 0.500000, 0.500000, 0.000000)
    //return t3;
}

vertex VertexOut vertexPCT(const VertexInPCT vertex_array   [[stage_in]],
                              const device Uniforms& uniforms       [[buffer(1)]],
                              unsigned int vid [[vertex_id]]) {
    VertexOut vertexOut;
    
    vertexOut.position = uniforms.projMatrix * uniforms.ViewMatrix * uniforms.modelMatrix * float4(vertex_array.position, 1);
    vertexOut.fragmentPosition = (uniforms.modelMatrix * float4(vertex_array.position, 1.0)).xyz;
    //vertexOut.normal = (uniforms.ViewMatrix * uniforms.modelMatrix * float4(vertex_array.normal, 0.0)).xyz;
    vertexOut.normal = float3(0, 0, 1);
    vertexOut.texCoord = vertex_array.texCoord;
    
    return vertexOut;
}

vertex VertexOut vertexDefault(const VertexInDefault vertex_array   [[stage_in]],
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

fragment half4 fragmentDefault(VertexOut out [[stage_in]],
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
