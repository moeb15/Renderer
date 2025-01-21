cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix viewProj;
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
    
    vso.viewPos = mul(input.position, world);
    vso.viewPos = mul(vso.viewPos, view);
    
    vso.normal = mul(input.normal, (float3x3) world);
    vso.normal = mul(vso.normal, (float3x3) view);
    
    return vso;
}