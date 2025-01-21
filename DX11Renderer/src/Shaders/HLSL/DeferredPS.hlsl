Texture2D baseTexture       : register(t0);
Texture2D normalMap         : register(t1);
SamplerState clampSampler   : register(s0);
SamplerState wrapSampler    : register(s1);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 viewPos : VIEWPOS;
};

struct PSOut
{
    float4 diffuse  : SV_TARGET0;
    float4 normal   : SV_TARGET1;
    float4 depth    : SV_TARGET2;
    float4 vPos     : SV_TARGET3;
};

PSOut main(PSIn input) 
{
    PSOut pso;
    float depthValue;
    float4 normals;
    
    depthValue = input.position.z / input.position.w;

    normals = normalMap.Sample(wrapSampler, input.uv);
    normals.x = normals.x * 2.0f - 1.0f;
    normals.y = -normals.y * 2.0f - 1.0f;
    normals.z = -normals.z;
    
    pso.diffuse = baseTexture.Sample(wrapSampler, input.uv);
    pso.normal = float4(input.normal.xyz, 1.0f);
    pso.depth = float4(depthValue, depthValue, depthValue, 1.0f);
    pso.vPos = float4(input.viewPos.xyz, 1.0f);
   
    
    return pso;
}