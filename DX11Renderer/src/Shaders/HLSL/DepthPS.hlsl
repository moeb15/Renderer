#include "Common.hlsli"

Texture2D albedoTex         : register(t0);
SamplerState wrapSampler    : register(s1);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 depthPos : DEPTHPOS;
};

float main(PSIn input) : SV_Depth
{
    float4 albedo = albedoTex.Sample(wrapSampler, input.uv);
    
#ifdef ALPHATEST
    clip(albedo.a < 0.1f ? -1 : 1);
#endif
    
    float depthValue;
    depthValue = input.depthPos.z / input.depthPos.w;
    depthValue += 0.0001f;
    
    return depthValue;
}