cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix proj;
    row_major matrix viewProj;
};

cbuffer LightBuffer
{
    row_major matrix lightViewProj;
    float3 lightPosition;
    float2 padding;
};

struct VSIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 instancePosition : INSTANCEPOS;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 viewPos : VIEWPOS;
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
    
    vso.viewPos = vso.position;
    
    return vso;
}