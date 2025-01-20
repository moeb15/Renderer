#include "Common.hlsli"

Texture2D albedoMap			: register(t0);
Texture2D normalMap			: register(t1);
Texture2D specularMap		: register(t2);
Texture2D shadowMap         : register(t4);
SamplerState clampSampler	: register(s0);
SamplerState wrapSampler	: register(s1);

cbuffer LightPropertiesBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
    float2 padding2;
};

cbuffer TransparencyBuffer : register(b1)
{
    float blendAmount;
    float3 padding;
};

cbuffer LightDirectionBuffer : register(b2)
{
    float3 lightDirection;
    float padding0;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 lightPos : LIGHTPOS;
    float4 viewPos : VIEWPOS;
};

float CalcShadowValue(PSIn input);

float4 main(PSIn input) : SV_TARGET
{
    float4 albedoColor, normalColor, specmapColor;
    float lIntensity;
    float specIntensity;
    float4 specular;
    float3 reflection;
    float4 finalColor;
    
    specIntensity = 0.0f;
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    albedoColor = albedoMap.Sample(wrapSampler, input.uv);
    
#ifdef ALPHATEST
    clip(albedoColor.a < 0.1f ? -1.f : 1.f);
#endif
    normalColor = normalMap.Sample(wrapSampler, input.uv);
    specmapColor = specularMap.Sample(wrapSampler, input.uv);
    
    normalColor.x = normalColor.x * 2.0f - 1.0f;
    normalColor.y = -normalColor.y * 2.0f - 1.0f;
    normalColor.z = -normalColor.z;
    
    finalColor = ambientColor;
    
    lIntensity = saturate(dot(-lightDirection, normalColor.xyz));

    finalColor += diffuseColor * lIntensity;
    finalColor = saturate(finalColor);
        
    reflection = normalize(2.0f * lIntensity * normalColor.xyz - lightDirection);
    specIntensity = pow(saturate(dot(reflection, input.viewDir)), specularPower);
    specular = specIntensity * specularColor * specmapColor;
    
    float shadowValue = CalcShadowValue(input);
    
    finalColor *= albedoColor;
    finalColor = saturate(finalColor + specular);
    finalColor *= shadowValue;
    finalColor.a = blendAmount;
    
    return finalColor;
}

float4 CalcDirLight(float3 normal, float3 viewDir)
{
    float lIntensity = saturate(dot(normal, -lightDirection));
    float3 reflection = normalize(2.0f * lIntensity * normal - lightDirection);
    float specIntensity = pow(saturate(dot(viewDir, reflection)), specularPower);
    float4 specular = specIntensity * specularColor;
    
    return saturate(ambientColor + diffuseColor * lIntensity + specular);
}

float CalcShadowValue(PSIn input)
{
    float2 projectedUV;
    float shadowValue;
    
    projectedUV.x = (input.viewPos.x / input.viewPos.w + 1.0f) / 2.0f;
    projectedUV.y = (-input.viewPos.y / input.viewPos.w + 1.0f) / 2.0f;
    
    shadowValue = shadowMap.Sample(clampSampler, projectedUV).r;
    
    return saturate(shadowValue + 0.25f);
}