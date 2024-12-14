struct PSIn
{
    float4 position : SV_POSITION;
};

float4 main(PSIn input) : SV_TARGET
{
    return float4(saturate(input.position.xyz), 1.0f);
}