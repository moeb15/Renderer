struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PSIn input) : SV_TARGET
{
    return saturate(input.color + 0.15f);
}