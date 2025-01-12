cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix viewProj;
};

cbuffer LightBuffer
{
    row_major matrix lightViewProj;
    float3 lightPosition;
    float padding;
};

struct VSIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 instancePosition : INSTANCEPOS;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 lightViewPos : VIEWPOS;
    float3 lightPos : LIGHTPOS;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    input.position.w = 1.0f;
    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;
    
    vso.position = mul(input.position, world);
    vso.position = mul(vso.position, viewProj);
    
    vso.uv = input.uv;
    
    vso.normal = mul(input.normal, (float3x3) world);
    vso.normal = normalize(vso.normal);
    
    float4 worldPos = mul(input.position, world);
    vso.lightPos = lightPosition.xyz - worldPos.xyz;
    vso.lightPos = normalize(vso.lightPos);
    
    vso.lightViewPos = mul(input.position, world);
    vso.lightViewPos = mul(vso.lightViewPos, lightViewProj);
    
    return vso;
}