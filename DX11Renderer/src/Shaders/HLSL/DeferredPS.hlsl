#include "Common.hlsli"

Texture2D baseTexture       : register(t0);
Texture2D normalMap         : register(t1);
SamplerState clampSampler   : register(s0);
SamplerState wrapSampler    : register(s1);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 viewPos : VIEWPOS;
};

struct PSOut
{
    float4 normal   : SV_TARGET0;
    float4 vPos     : SV_TARGET1;
};

PSOut main(PSIn input) 
{
    PSOut pso;
    float4 normals;
    float4 diffuse;
    
    diffuse = baseTexture.Sample(wrapSampler, input.uv);
    
    //normals = normalMap.Sample(clampSampler, input.uv);
    //normals.x = normals.x * 2.0f - 1.0f;
    //normals.y = -normals.y * 2.0f - 1.0f;
    //normals.z = -normals.z;
    
    input.normal = normalize(input.normal);
    
    pso.normal = float4(input.normal.xyz, 1.0f);
    pso.vPos = float4(input.viewPos.xyz, diffuse.a);
   
    
    return pso;
}