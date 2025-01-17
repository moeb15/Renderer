Texture2D albedoMap			: register(t0);
Texture2D normalMap			: register(t1);
Texture2D specularMap		: register(t2);
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
};

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
    normalColor = normalMap.Sample(wrapSampler, input.uv);
    specmapColor = specularMap.Sample(wrapSampler, input.uv);
    
    normalColor.x = normalColor.x * 2.0f - 1.0f;
    normalColor.y = -normalColor.y * 2.0f - 1.0f;
    normalColor.z = -normalColor.z;
    normalColor = normalize(normalColor);
    
    finalColor = ambientColor;
    
    lIntensity = saturate(dot(-lightDirection, input.normal));
    if (lIntensity > 0.0f)
    {
        finalColor += diffuseColor * lIntensity;
        finalColor = saturate(finalColor);
        
        reflection = normalize(2.0f * lIntensity * input.normal - lightDirection);
        specIntensity = pow(saturate(dot(reflection, input.viewDir)), specularPower);
        specular = specIntensity * specularColor * specmapColor;
    }
    
    finalColor *= albedoColor;
    finalColor = saturate(finalColor + specular);
    finalColor.a = blendAmount;
    
    return finalColor;
}