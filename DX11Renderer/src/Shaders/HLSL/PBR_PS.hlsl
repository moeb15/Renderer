Texture2D albedoMap			: register(t0);
Texture2D normalMap			: register(t1);
Texture2D roughnessMap		: register(t2);
SamplerState wrapSampler	: register(s0);

cbuffer LightBuffer : register(b2)
{
    float3 lightDirection;
    float padding;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};

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
    lDir = normalize(lDir);
    
    albedo = albedoMap.Sample(wrapSampler, input.uv).rgb;
    rmColor = roughnessMap.Sample(wrapSampler, input.uv).rgb;
    bumpMap = normalMap.Sample(wrapSampler, input.uv).rgb;
    
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    bumpNormal = normalize(bumpNormal);
    
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
    denominator = 3.14159265f * (denominator * denominator);
    normalDist = roughSqr2 / denominator;
    
    // Schlick geometric shadow calculation
    smithL = NdotL / (NdotL * (1.0f - roughnessSqr) + roughnessSqr);
    smithV = NdotV / (NdotV * (1.0f - roughnessSqr) + roughnessSqr);
    geometricshadow = smithL * smithV;
    
    // fresnel schlick 
    fresnel = f0 + (1.0f - f0) * pow(1.0f - HdotV, 5.0f);
    
    // dfg finished, now calculate denominator
    specularity = (normalDist * fresnel * geometricshadow) / (4.0f * (NdotL * NdotV) + 0.00001f);
    
    // final light equation
    color.rgb = albedo + specularity;
    color.rgb = color.rgb * NdotL;
    
    color = float4(color.rgb, 1.0f);

    return color;
}
