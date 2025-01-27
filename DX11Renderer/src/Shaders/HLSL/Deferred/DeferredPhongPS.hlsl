#include "../Common.hlsli"

Texture2D albedoMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D shadowMap : register(t4);
Texture2D ambientMap : register(t6);
SamplerState clampSampler : register(s0);
SamplerState wrapSampler : register(s1);

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

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 viewDir : VIEWDIR;
    float4 viewPos : VIEWPOS;
    float4 worldPos : WORLDPOS;
};

cbuffer LightDirectionBuffer : register(b2)
{
    float3 lightDirection;
    float padding0;
};

cbuffer PointLightData : register(b3)
{
    PointLight pointLight[MAX_LIGHTS];
};

float CalcShadowValue(PSIn input);
float4 CalcDirLight(PSIn input, float3 normal, float3 viewDir);

float4 main(PSIn input) : SV_TARGET
{
    float4 finalColor;
    float3 normal = normalMap.Sample(wrapSampler, input.uv).xyz;
    float shadow = CalcShadowValue(input);
    
    finalColor = CalcDirLight(input, normal, input.viewDir);
    finalColor *= shadow;
    finalColor.a = blendAmount;
    
    return finalColor;
}

float4 CalcDirLight(PSIn input, float3 normal, float3 viewDir)
{
    float lIntensity = saturate(dot(normal, -lightDirection));
    float3 reflection = reflect(-lightDirection, normal);
    float specIntensity = pow(saturate(dot(viewDir, reflection)), specularPower);
    
    float4 ambient = ambientMap.Sample(clampSampler, input.uv).r * albedoMap.Sample(wrapSampler, input.uv);
    float4 diffuse = diffuseColor * lIntensity * albedoMap.Sample(wrapSampler, input.uv);
    float4 specular = specularColor * specIntensity * specularMap.Sample(wrapSampler, input.uv);
    
    return saturate(ambient + diffuse + specular);
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