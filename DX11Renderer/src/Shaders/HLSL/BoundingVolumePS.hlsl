Texture2D baseTex           : register(t0);
SamplerState wrapSampler    : register(s1);


struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 texColor = baseTex.Sample(wrapSampler, input.uv);
    
    if (texColor.x <= 0.1f && texColor.y <= 0.1f && texColor.z <= 0.1f)
    {
        clip(-1);
    }
    
    return texColor;
}