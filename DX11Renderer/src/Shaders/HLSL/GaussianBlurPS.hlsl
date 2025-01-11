Texture2D baseTexture :			register(t0);
SamplerState wrapSampler :		register(s0);

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
    float weights[5];
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
    
    weights[0] = 1.0f;
    weights[1] = 0.9f;
    weights[2] = 0.55f;
    weights[3] = 0.18f;
    weights[4] = 0.1f;
    
    normalization = (weights[0] + 2.0f * (weights[1] + weights[2] + weights[3] + weights[4]));
    
    for (i = 0; i < 5; i++)
    {
        weights[i] /= normalization;
    } 
    
    finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    finalColor += texColor * weights[0];
    
    for (i = 0; i < 4; i++)
    {
        finalColor += baseTexture.Sample(wrapSampler, blurLeft[i]) * weights[4 - i];
        finalColor += baseTexture.Sample(wrapSampler, blurRight[i]) * weights[i + 1];
    }
    
    finalColor.a = 1.0f;
    
    return finalColor;
}