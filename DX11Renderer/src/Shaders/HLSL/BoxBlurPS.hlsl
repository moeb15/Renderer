Texture2D baseTexture : register(t0);
SamplerState wrapSampler : register(s0);

cbuffer ScreenBuffer
{
    float screenWidth;
    float screenHeight;
    float blurType;
    float padding;
};

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSIn input) : SV_TARGET
{
    float texelSize;
    float2 blurLeft[4];
    float2 blurRight[4];
    float4 texColor;
    float4 finalColor;
    float normalization;
    int i;
    
    texColor = baseTexture.Sample(wrapSampler, input.uv);
    
    if (blurType < 0.1f)
    {
        texelSize = 1.0f / screenWidth;
        
        for (i = 0; i < 4; i++)
        {
            blurLeft[i] = input.uv + float2(texelSize * -(i + 1), 0.0f);
            blurRight[i] = input.uv + float2(texelSize * (i + 1), 0.0f);
        }
    }
    else
    {
        texelSize = 1.0f / screenHeight;
        
        for (i = 0; i < 4; i++)
        {
            blurLeft[i] = input.uv + float2(0.0f, texelSize * -(i + 1));
            blurRight[i] = input.uv + float2(0.0f, texelSize * (i + 1));
        }
    }
    
    finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    finalColor += texColor;
    
    for (i = 0; i < 4; i++)
    {
        finalColor += baseTexture.Sample(wrapSampler, blurLeft[i]);
        finalColor += baseTexture.Sample(wrapSampler, blurRight[i]);
    }
    
    finalColor /= 9.f;
    finalColor = saturate(finalColor);
    
    finalColor.a = 1.0f;
    
    return finalColor;
}