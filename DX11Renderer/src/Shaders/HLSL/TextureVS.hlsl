cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix viewProj;
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
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    vso.position = mul(float4(input.position.xyz, 1.0f), world);
    vso.position = mul(vso.position, viewProj);
    
    vso.uv = input.uv;
    
	return vso;
}