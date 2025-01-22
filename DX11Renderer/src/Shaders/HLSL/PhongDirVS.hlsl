cbuffer MatrixBuffer : register(b0)
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
    float padding;
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
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 lightPos : LIGHTPOS;
    float4 viewPos : VIEWPOS;
};

VSOut main(VSIn input)
{
    VSOut vso;
    float4 worldPos;
    
    input.position.w = 1.0f;
    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;
    
    vso.position = mul(input.position, world);
    vso.position = mul(vso.position, viewProj);
    
    vso.uv = input.uv;
    
    vso.normal = mul(input.normal, (float3x3) world);
    vso.normal = normalize(vso.normal);
    
    vso.binormal = mul(input.binormal, (float3x3) world);
    vso.binormal = normalize(vso.binormal);
    
    vso.tangent = mul(input.tangent, (float3x3) world);
    vso.tangent = normalize(vso.tangent);
    
    worldPos = mul(input.position, world);
    
    vso.viewDir = cameraPos.xyz - worldPos.xyz;
    vso.viewDir = normalize(vso.viewDir);
        
    vso.lightPos = lightPosition.xyz - worldPos.xyz;
    vso.lightPos = normalize(vso.lightPos);
    
    vso.viewPos = vso.position;
    
    return vso;
}