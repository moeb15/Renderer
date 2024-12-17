Texture2D baseTex :				register(t0);
SamplerState baseSampler :		register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    return baseTex.Sample(baseSampler, input.uv);
}