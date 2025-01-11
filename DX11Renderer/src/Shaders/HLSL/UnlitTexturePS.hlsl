Texture2D baseTex : register(t0);
SamplerState baseSampler : register(s0);


struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 texColor = baseTex.Sample(baseSampler, input.uv);
    return texColor;
}