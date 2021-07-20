cbuffer WorldData : register(b0)
{
    float4x4 projection;
    float4x4 view;
};

cbuffer ObjectData : register(b1)
{
    float4x4 model;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PSInput VSMain(
        float3 position : SV_POSITION,
        float3 normal : NORMAL,
        float2 color : COLOR,
        float2 uv : TEXCOORD0,
        float3 tangent : TANGENT
)
{
    PSInput result;

    float4 newPosition = float4(position, 1.0f);

    newPosition = mul(newPosition, model);
    newPosition = mul(newPosition, view);
    newPosition = mul(newPosition, projection);

    result.position = newPosition;
    result.uv = uv;

    return result;
}