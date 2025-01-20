#include "Common.hlsli"

Texture2D albedoMap			: register(t0);
Texture2D normalMap			: register(t1);
Texture2D roughnessMap		: register(t3);
Texture2D shadowMap         : register(t4);
SamplerState clampSampler	: register(s0);
SamplerState wrapSampler	: register(s1);

cbuffer LightPropsBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
    float2 padding;
};

cbuffer LightBuffer : register(b2)
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

// cook-torrance brdf
float4 main(PSIn input) : SV_TARGET
{
    float3 lDir;
    float3 albedo, rmColor, bumpMap;
    float3 bumpNormal;
    float roughness, metallic;
    float3 f0;
    float3 halfDir;
    float NdotH, NdotV, NdotL, HdotV;
    float roughnessSqr, roughSqr2, NdotHSqr, denominator, normalDist;
    float smithL, smithV, geometricshadow;
    float3 fresnel;
    float3 specularity;
    float4 color;
    
    lDir = -lightDirection;
    
    albedo = albedoMap.Sample(wrapSampler, input.uv).rgb;
    float albedoAlpha = albedoMap.Sample(wrapSampler, input.uv).a;
#ifdef ALPHATEST
    clip(albedoAlpha < 0.1f ? -1 : 1);
#endif
    
    rmColor = roughnessMap.Sample(wrapSampler, input.uv).rgb;
    bumpMap = normalMap.Sample(wrapSampler, input.uv).rgb;
    
    bumpMap.x = (bumpMap.x * 2.0f) - 1.0f;
    bumpMap.y = (-bumpMap.y * 2.0f) - 1.0f;
    bumpMap.z = -bumpMap.z;
    
    bumpNormal = normalize(bumpMap);
    //bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    //bumpNormal = normalize(bumpNormal);
    
    roughness = rmColor.r;
    metallic = rmColor.g;
    
    f0 = float3(0.04f, 0.04f, 0.04f);
    f0 = lerp(f0, albedo, metallic);
    
    halfDir = normalize(input.viewDir + lDir);
    NdotH = max(0.0f, dot(bumpNormal, halfDir));
    NdotV = max(0.0f, dot(bumpNormal, input.viewDir));
    NdotL = max(0.0f, dot(bumpNormal, lDir));
    HdotV = max(0.0f, dot(halfDir, input.viewDir));
    
    // GGX normal dist
    roughnessSqr = roughness * roughness;
    roughSqr2 = roughnessSqr * roughnessSqr;
    NdotHSqr = NdotH * NdotH;
    denominator = (NdotHSqr * (roughSqr2 - 1.0f) + 1.0f);
    denominator = PI * (denominator * denominator);
    normalDist = roughSqr2 / denominator;
    
    // Schlick geometric shadow calculation
    smithL = NdotL / (NdotL * (1.0f - roughnessSqr) + roughnessSqr + 0.00001f);
    smithV = NdotV / (NdotV * (1.0f - roughnessSqr) + roughnessSqr + 0.00001f);
    geometricshadow = smithL * smithV;
    
    // fresnel schlick 
    fresnel = f0 + (1.0f - f0) * pow(1.0f - HdotV, 5.0f);
    
    // dfg finished, now calculate denominator
    specularity = (normalDist * fresnel * geometricshadow) / (4.0f * (NdotL * NdotV) + 0.00001f);
    
    float shadowValue = CalcShadowValue(input);
    float lIntensity = saturate(dot(bumpNormal, input.lightPos));
    // final light equation
    float3 ambient = albedo;
    float3 diffuse =  albedo * (1.0f - fresnel) * NdotL;
    color.rgb = (ambient + diffuse + specularity) * shadowValue;
    color.rgb = color.rgb * NdotL;
    
    color = float4(color.rgb, 1.0f);
    color = saturate(color);
    return color;
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