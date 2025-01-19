Texture2D depthMapTexture	: register(t4);
SamplerState clampSampler	: register(s0);

cbuffer LightPropsBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float specularPower;
    float bias;
    float2 padding;
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
    
    finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    projectedUV.x = (input.lightViewPos.x / input.lightViewPos.w + 1.0f) / 2.0f;
    projectedUV.y = (-input.lightViewPos.y / input.lightViewPos.w + 1.0f) / 2.0f;
    
    if ((saturate(projectedUV.x) == projectedUV.x) && (saturate(projectedUV.y) == projectedUV.y))
    {
        depthValue = depthMapTexture.Sample(clampSampler, projectedUV).r;
        
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue -= bias;
        
        if (lightDepthValue < depthValue)
        {
            lightIntensity = saturate(dot(input.normal, input.lightPos));
            
            if (lightIntensity > 0.0f)
            {
                finalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
    }
    
    return finalColor;
}