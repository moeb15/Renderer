Texture2D baseTexture :     register(t0);
SamplerState wrapSampler :  register(s1);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSOut
{
    float4 diffuse : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 depth : SV_TARGET2;
};

PSOut main(PSIn input) 
{
    PSOut pso;
    float depthValue;
    depthValue = input.position.z / input.position.w;
    
    pso.diffuse = baseTexture.Sample(wrapSampler, input.uv);
    pso.normal = float4(input.normal, 1.0f);
    pso.depth = float4(depthValue, depthValue, depthValue, 1.0f);
    
    return pso;
}