cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix viewProj;
};

cbuffer LightPosBuffer
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
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 lightDirection : LIGHTDIR;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    vso.position = mul(float4(input.position.xyz, 1.0f), world);
    vso.position = mul(vso.position, viewProj);
    
    vso.uv = input.uv;
    
    float4 worldPosition = mul(float4(input.position.xyz, 1.0f), world);
    
    vso.lightDirection = lightPosition - worldPosition.xyz;
    vso.lightDirection = normalize(vso.lightDirection);
    
    vso.normal = mul(input.normal, (float3x3) world);
    vso.normal = normalize(vso.normal);
    
	return vso;
}