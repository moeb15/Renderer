Texture2D shadowMap : register(t0);
RWTexture2D<float4> shadowAtlas : register(u0);

cbuffer ShadowMapOffsets
{
    float2 offsets;
    float2 atlasSize;
};

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint2 shadowCoord = DTid.xy;
    
    if (shadowCoord.x >= atlasSize.x || shadowCoord.y >= atlasSize.y)
        return;
    
    float shadowValue = shadowMap.Load(int3(shadowCoord, 0));
    
    uint2 atlasCoord = shadowCoord + offsets;
    
    shadowAtlas[atlasCoord] = shadowValue;
}