Texture2D baseTex : register(t0);
Texture2D ambientTex : register(t1);
SamplerState wrapSampler : register(s0);


struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 ambientColor = ambientTex.Sample(wrapSampler, input.uv);
    float4 texColor = baseTex.Sample(wrapSampler, input.uv);
    
    texColor.rgb *= ambientColor.r;
    
    return texColor;
}