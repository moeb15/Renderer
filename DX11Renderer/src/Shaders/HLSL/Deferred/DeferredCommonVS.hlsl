cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix proj;
    row_major matrix viewProj;
};

cbuffer LightBuffer : register(b1)
{
    row_major matrix lightViewProj;
    float3 lightPosition;
    float2 padding;
};

cbuffer CameraBuffer : register(b2)
{
    float3 cameraPos;
    float padding0;
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
    float3 viewDir : VIEWDIR;
    float4 viewPos : VIEWPOS;
    float4 worldPos : WORLDPOS;
};

VSOut main(VSIn input)
{
    VSOut vso;
    float4 worldPos;
    
    input.position.w = 1.0f;
    vso.position = mul(input.position, world);
    vso.position = mul(vso.position, viewProj);
    vso.position.z = 0.0f;
    
    vso.uv = input.uv;
    
    worldPos = mul(input.position, world);
    
    vso.worldPos = worldPos;
    vso.viewDir = cameraPos.xyz - worldPos.xyz;
    vso.viewDir = normalize(vso.viewDir);
    
    vso.viewPos = vso.position;
    
    return vso;
}