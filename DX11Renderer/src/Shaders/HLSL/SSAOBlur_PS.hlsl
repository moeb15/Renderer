Texture2D ssaoTex           : register(t0);
Texture2D normalTex         : register(s1);
SamplerState pointSampler   : register(s2);

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
    float2 texOffset;
    int radius;
    float colorSum;
    float4 centerDepth;
    float2 tex;
    float4 nDepth;
    float ssaoVal;
    float weight;
    float weightSum;
    
    if (blurType < 0.1f)
    {
        texelSize = 1.0f / screenWidth;
        texOffset = float2(texelSize, 0.0f);
    }
    else
    {
        texelSize = 1.0f / screenHeight;
        texOffset = float2(0.0f, texelSize);
    }
    
    radius = 5;
    
    // Set an array of weights for how much a pixel contributes to the blur.
    float weightArray[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
    colorSum = weightArray[radius] * ssaoTex.SampleLevel(pointSampler, input.uv, 0).r;
    weightSum = weightArray[radius];
    
    centerDepth = normalTex.SampleLevel(pointSampler, input.uv, 0);
    
    for (int i = -radius ; i <= radius; i++)
    {
        if (i == 0)
            continue;
        
        tex = input.uv + i * texOffset;
        nDepth = normalTex.SampleLevel(pointSampler, tex, 0);
        if (dot(nDepth.xyz, centerDepth.xyz) >= 0.8f)
        {
            ssaoVal = ssaoTex.SampleLevel(pointSampler, tex, 0).r;
            
            weight = weightArray[i + radius];
            colorSum += (ssaoVal * weight);
            weightSum += weight;
        }
    }
    colorSum /= weightSum;
    
    return float4(colorSum, colorSum, colorSum, 1.0f);
}