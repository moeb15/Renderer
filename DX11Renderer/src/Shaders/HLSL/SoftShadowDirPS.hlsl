#include "Common.hlsli"

Texture2D baseTex : register(t0);
Texture2D shadowMapTex : register(t4);
SamplerState clampSampler : register(s0);
SamplerState wrapSampler : register(s1);

cbuffer LightPropsBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
    float2 padding;
};

cbuffer TransparencyBuffer : register(b1)
{
    float blendAmount;
    float3 padding0;
};

cbuffer LightDirectionBuffer : register(b2)
{
    float3 lightDirection;
    float padding1;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 viewPos : VIEWPOS;
    float3 lightPos : LIGHTPOS;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 finalColor;
    float4 texColor;
    float2 projectedUV;
    float shadowValue;
    float lightIntensity;
    float3 lightDir;
    
    lightDir = -lightDirection;
    
    texColor = baseTex.Sample(wrapSampler, input.uv);
    
#ifdef ALPHATEST
    clip(texColor.a < 0.1f ? -1 : 1);
#endif
    
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    finalColor = ambientColor;
    finalColor += diffuseColor * lightIntensity;
    finalColor = saturate(finalColor);
    finalColor *= texColor;
    
    projectedUV.x = (input.viewPos.x / input.viewPos.w + 1.0f) / 2.0f;
    projectedUV.y = (-input.viewPos.y / input.viewPos.w + 1.0f) / 2.0f;
    
    shadowValue = shadowMapTex.Sample(clampSampler, projectedUV).r;
    finalColor *= (shadowValue + 0.25f);
    finalColor.a = blendAmount;
    
    return finalColor;
}