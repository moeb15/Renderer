Texture2D baseTexture		: register(t0);
Texture2D depthMapTexture	: register(t1);
SamplerState clampSampler	: register(s0);
SamplerState wrapSampler	: register(s1);

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 lightViewPos : VIEWPOS;
    float3 lightPos : LIGHTPOS;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 finalColor;
    float2 projectedUV;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 texColor;
    
    finalColor = ambientColor;
    
    projectedUV.x = (input.lightViewPos.x / input.lightViewPos.w + 1.0f) / 2.0f;
    projectedUV.y = (-input.lightViewPos.y / input.lightViewPos.w + 1.0f) / 2.0f;
    
    if (saturate(projectedUV).x == projectedUV.x && saturate(projectedUV).y == projectedUV.y)
    {
        depthValue = depthMapTexture.Sample(clampSampler, projectedUV).r;
        
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue = lightDepthValue - bias;
        
        if (lightDepthValue < depthValue)
        {
            lightIntensity = saturate(dot(input.normal, input.lightPos));
            
            finalColor += (diffuseColor * lightIntensity);
            finalColor = saturate(finalColor);
        }
    }
    
    texColor = baseTexture.Sample(wrapSampler, input.uv);
    
    finalColor *= texColor;
    
    return finalColor;
}