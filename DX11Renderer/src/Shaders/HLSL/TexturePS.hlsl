Texture2D baseTex :				register(t0);
SamplerState baseSampler :		register(s0);

cbuffer LightPropsBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
};

cbuffer TransparencyBuffer
{
    float blendAmount;
    float3 padding;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 lightDirection : LIGHTDIR;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 texColor;
    float lIntensity;
    float4 finalColor;
    
    texColor = baseTex.Sample(baseSampler, input.uv);
    
    finalColor = ambientColor;
    
    lIntensity = saturate(dot(input.normal, input.lightDirection));
    if (lIntensity > 0.0f)
    {
        finalColor += diffuseColor * lIntensity;
    }
    
    finalColor *= texColor;
    finalColor = saturate(finalColor);
    
    finalColor.a = blendAmount;
    
    return finalColor;
}