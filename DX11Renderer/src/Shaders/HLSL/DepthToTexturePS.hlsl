struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 depthPos : DEPTHPOS;
};

float4 main(PSIn input) : SV_TARGET
{
    float depthValue;
    float4 color;
    
    depthValue = input.depthPos.z / input.depthPos.w;
    color = float4(depthValue, depthValue, depthValue, 1.0f);
    
    return color;
}