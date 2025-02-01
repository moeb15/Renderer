#include "Common.hlsli"

Texture2D baseTexture       : register(t0);
Texture2D normalMap         : register(t1);
Texture2D specularMap       : register(t2);
Texture2D rmMap             : register(t3);
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
    float4 normal       : SV_TARGET0;
    float4 vPos         : SV_TARGET1;
    float4 color        : SV_TARGET2;
    float4 specular     : SV_TARGET3;
    float4 roughness    : SV_TARGET4;
    float4 metallic     : SV_TARGET5;
};

PSOut main(PSIn input) 
{
    PSOut pso;
    float4 normals;
    float4 diffuse;
    float4 specular;
    float4 color;
    float4 rmTex;
    float roughness;
    float metallic;
    
    specular = specularMap.Sample(wrapSampler, input.uv);
    color = baseTexture.Sample(wrapSampler, input.uv);
    rmTex = rmMap.Sample(wrapSampler, input.uv);
    
    roughness = rmTex.r;
    metallic = rmTex.g;
    
    normals = normalMap.Sample(wrapSampler, input.uv);
    //normals.x = normals.x * 2.0f - 1.0f;
    //normals.y = -normals.y * 2.0f - 1.0f;
    //normals.z = -normals.z;
    //normals = normalize(normals);
    
    input.normal = normalize(input.normal);
    
    pso.normal = float4(input.normal.xyz, 1.0f);
    pso.vPos = float4(input.viewPos.xyz, diffuse.a);
    pso.color = color;
    pso.specular = specular;
    pso.roughness = float4(roughness, roughness, roughness, 1.0f);
    pso.metallic = float4(metallic, metallic, metallic, 1.0f);
   
    
    return pso;
}