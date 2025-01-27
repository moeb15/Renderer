#include "Common.hlsli"

Texture2D albedoMap			: register(t0);
Texture2D normalMap			: register(t1);
Texture2D specularMap		: register(t2);
Texture2D shadowMap         : register(t4);
Texture2D ambientMap        : register(t6);
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

cbuffer PointLightData  : register(b3)
{
    PointLight pointLight[MAX_LIGHTS];
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
    float4 worldPos : WORLDPOS;
};

float CalcShadowValue(PSIn input);
float4 CalcDirLight(PSIn input, float3 normal, float3 viewDir);
float3 CalcPointLight(PSIn input, int index);

float4 main(PSIn input) : SV_TARGET
{
    float4 albedoColor, specmapColor;
    float4 finalColor;
    float3 normalColor;
    float ambient;
    
    albedoColor = albedoMap.Sample(wrapSampler, input.uv);
#ifdef ALPHATEST
    clip(albedoColor.a < 0.1f ? -1.f : 1.f);
#endif
    normalColor = normalMap.Sample(wrapSampler, input.uv).xyz;
    normalColor = normalize(normalColor);
    
    float shadowValue = CalcShadowValue(input);

    finalColor = CalcDirLight(input, normalColor, input.viewDir);
    
    
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (pointLight[i].enabled == 1)
        {
            float3 plDiff = CalcPointLight(input, i);
            finalColor.xyz += plDiff;
        }
    }
    
    finalColor *= shadowValue;
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

float3 CalcPointLight(PSIn input, int index)
{
    float3 lightDir = normalize(pointLight[index].position - input.worldPos.xyz);
    float angle = dot(lightDir, normalize(lightDirection));
    float lightIntensity = saturate(dot(input.normal, lightDir));
    
    float3 reflectDir = reflect(-lightDir, input.normal);
    float specIntensity = pow(saturate(dot(input.viewDir, reflectDir)), specularPower);
    
    float dist = length(pointLight[index].position - input.worldPos.xyz);
    float attenuation = 1.0f / (1.0f + 0.7f * dist + 1.8f * dist * dist);
    attenuation *= 0.1f;
    
    float3 ambient = pointLight[index].colour * ambientMap.Sample(clampSampler, input.uv).r * albedoMap.Sample(wrapSampler, input.uv);
    float3 diffuse = pointLight[index].colour * lightIntensity * albedoMap.Sample(wrapSampler, input.uv);
    float3 specular = specIntensity * specularMap.Sample(wrapSampler, input.uv);
    
    diffuse *= attenuation;
    ambient *= attenuation;
    specular *= attenuation;
    
    return (diffuse + ambient + specular);
}
