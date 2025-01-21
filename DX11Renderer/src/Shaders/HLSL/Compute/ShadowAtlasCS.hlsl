#include "../Common.hlsli"

Texture2DArray shadowMap : register(t0);
RWTexture2D<float4> shadowAtlas : register(u0);

cbuffer ShadowMapOffsets
{
    float2 offsets[64];
};

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int lightIndex = DTid.x + DTid.y * 16;
    
    if (lightIndex < 64)
    {
        int2 offset = (int2) offsets[lightIndex];
        shadowAtlas[DTid.xy + offset.xy] = shadowMap.Load(int4(DTid.xy, DTid.x, 0));
    }
}