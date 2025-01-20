Texture2D baseTexture       : register(t0);
Texture2D normalMap         : register(t1);
Texture2D specularMap       : register(t2);
Texture2D roughnessMap      : register(t3);
SamplerState clampSampler   : register(s0);
SamplerState wrapSampler    : register(s1);

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSOut
{
    float4 diffuse  : SV_TARGET0;
    float4 normal   : SV_TARGET1;
    float4 spec     : SV_TARGET2;
    float4 rough    : SV_TARGET3;
    float4 metal    : SV_TARGET4;
    float4 depth    : SV_TARGET5;
};

PSOut main(PSIn input) 
{
    PSOut pso;
    float depthValue;
    float roughness, metallic;
    float4 rmMap;
    
    depthValue = input.position.z / input.position.w;
    rmMap = roughnessMap.Sample(wrapSampler, input.uv);
    roughness = rmMap.r;
    metallic = rmMap.g;
    
    pso.diffuse = baseTexture.Sample(wrapSampler, input.uv);
    pso.normal = normalMap.Sample(wrapSampler, input.uv);
    pso.spec = specularMap.Sample(wrapSampler, input.uv);
    pso.rough = float4(roughness, roughness, roughness, 1.0f);
    pso.metal = float4(metallic, metallic, metallic, 1.0f);
    pso.depth = float4(depthValue, depthValue, depthValue, 1.0f);
   
    
    return pso;
}