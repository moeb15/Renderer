#include "Common.hlsli"

Texture2D screenTex			: register(t0);
SamplerState wrapSampler	: register(s1);

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

bool EdgeDetected(float2 uv, inout float lumaCenter, inout float lumaUp,
                    inout float lumaDown, inout float lumaLeft, inout float lumaRight,
                    inout float lumaRange);
bool IsEdgeHorizontal(float2 uv, float lumaCenter, float lumaUp,
                    float lumaDown, float lumaLeft, float lumaRight,
                    inout float lumaDownUp, inout float lumaLeftRight,
                    inout float lumaLeftCorners, inout float lumaRightCorners);

float4 main(PSIn input) : SV_TARGET
{
    float4 texColor;
    float texelWidth;
    float texelHeight;
    float lumaCenter, lumaDown, lumaUp, lumaLeft, lumaRight, lumaRange;
    float lumaDownUp, lumaLeftRight, lumaLeftCorners, lumaRightCorners;
    texColor = screenTex.Sample(wrapSampler, input.uv);
    texelWidth = 1.0f / screenWidth;
    texelHeight = 1.0f / screenHeight;
    
    if (!EdgeDetected(input.uv, lumaCenter, lumaUp, lumaDown, lumaLeft, lumaRight, lumaRange)) return texColor;
    bool horizontal = IsEdgeHorizontal(input.uv, lumaCenter, lumaUp, lumaDown, lumaLeft, lumaRight,
                        lumaDownUp, lumaLeftRight, lumaLeftCorners, lumaRightCorners);
    
    // Select the two neighboring texels lumas in the opposite direction to the local edge
    float luma1 = horizontal ? lumaDown : lumaLeft;
    float luma2 = horizontal ? lumaUp : lumaRight;
    
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;
    
    bool is1Steepest = abs(gradient1) >= abs(gradient2);
    
    float gradientScaled = 0.25f * max(abs(gradient1), abs(gradient2));
    
    float stepLength = horizontal ? texelHeight : texelWidth;
    float lumaLocalAverage = 0.0f;
    
    if (is1Steepest)
    {
        stepLength -= stepLength;
        lumaLocalAverage = 0.5f * (luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5f * (luma2 + lumaCenter);
    }
    
    float2 currentUV = input.uv;
    if (horizontal)
    {
        currentUV.y += stepLength * 0.5f;
    }
    else
    {
        currentUV.x += stepLength * 0.5f;
    }
    
    float2 offset = horizontal ? float2(texelWidth, 0.0f) : float2(0.0f, texelHeight);
    //Comput UVs to explore on each side of the edge, orthogonally
    float2 uv1 = currentUV - offset;
    float2 uv2 = currentUV + offset;
    // Read the lumas at both current extremeites of the exploration segment, and compute the delta wrt local average luma
    float lumaEnd1 = RGB2Luma(screenTex.Sample(wrapSampler, uv1).rgb);
    float lumaEnd2 = RGB2Luma(screenTex.Sample(wrapSampler, uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;
    
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;
    
    if (!reached1)
    {
        uv1 -= offset;
    }
    if (!reached2)
    {
        uv2 += offset;
    }
    
    if (!reachedBoth)
    {
        for (int i = 2; i < ITERATIONS; i++)
        {
            if (!reached1)
            {
                lumaEnd1 = RGB2Luma(screenTex.Sample(wrapSampler, uv1).rgb);
                lumaEnd1 -= lumaLocalAverage;
            }
            
            if (!reached2)
            {
                lumaEnd2 = RGB2Luma(screenTex.Sample(wrapSampler, uv2).rgb);
                lumaEnd2 -= lumaLocalAverage;
            }
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;
            
            if (!reached1)
            {
                uv1 -= offset * QUALITY(i);
            }
            if (!reached2)
            {
                uv2 += offset * QUALITY(i);
            }
            
            if (reachedBoth) break;
        }

    }
    
    float distance1 = horizontal ? input.uv.x - uv1.x : input.uv.y - uv1.y;
    float distance2 = horizontal ? uv2.x - input.uv.x : uv2.y - input.uv.y;
    
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);
    
    float edgeThickness = (distance1 + distance2);
    
    float pixelOffset = -distanceFinal / edgeThickness + 0.5f;
    
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0f) != isLumaCenterSmaller;
    
    float finalOffset = correctVariation ? pixelOffset : 0.0f;
    
    // Sub-Pixel shifting
    float lumaAverage = (1.0 / 12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0, 1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
    // Compute a sub-pixel offset based on this delta.
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    // Pick the biggest of the two offsets.
    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    float2 finalUV = input.uv;
    if (horizontal)
    {
        finalUV.y += finalOffset * stepLength;
    }
    else
    {
        finalUV.x += finalOffset * stepLength;
    }
    
    float4 finalColor = screenTex.Sample(wrapSampler, finalUV);
    
    return finalColor;
}

bool EdgeDetected(float2 uv, inout float lumaCenter, inout float lumaUp,
                    inout float lumaDown, inout float lumaLeft, inout float lumaRight,
                    inout float lumaRange)
{
    float texelWidth;
    float texelHeight;
    float3 texColor;
    
    texelWidth = 1.f / screenWidth;
    texelHeight = 1.f / screenHeight;
    texColor = screenTex.Sample(wrapSampler, uv).rgb;
    
    // Luma at the current fragment
    lumaCenter = RGB2Luma(texColor);
    
    // Luma at the four direct neighbours of the current fragment
    lumaDown = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(0.0f, -texelHeight)).rgb);
    lumaUp = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(0.0f, texelHeight)).rgb);
    lumaLeft = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(-texelWidth, 0.0f)).rgb);
    lumaRight = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(texelWidth, 0.0f)).rgb);
    
    // Find the max and min luma around the current fragment
    float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));
    
    // Compute the delta
    lumaRange = lumaMax - lumaMin;
    
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
        return false;
    
    return true;
}

bool IsEdgeHorizontal(float2 uv, float lumaCenter, float lumaUp,
                    float lumaDown, float lumaLeft, float lumaRight,
                    inout float lumaDownUp, inout float lumaLeftRight,
                    inout float lumaLeftCorners, inout float lumaRightCorners)
{
    float texelWidth;
    float texelHeight;
    
    texelWidth = 1.f / screenWidth;
    texelHeight = 1.f / screenHeight;
    // Query 4 corners
    float lumaDownLeft = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(-texelWidth, -texelHeight)).rgb);
    float lumaDownRight = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(texelWidth, -texelHeight)).rgb);
    float lumaUpRight = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(texelWidth, texelHeight)).rgb);
    float lumaUpLeft = RGB2Luma(screenTex.Sample(wrapSampler, uv + float2(-texelWidth, texelHeight)).rgb);
    
    // combine the four egdes lumas
    lumaDownUp = lumaDown + lumaUp;
    lumaLeftRight = lumaLeft + lumaRight;
    
    // Same for corners
    lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;
    
    // compute an estimation of the gradient along the horizontal and vertical axis
    float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);
    
    return edgeHorizontal >= edgeVertical;
}
