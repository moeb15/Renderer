cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};


struct VSIn
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    vso.position = mul(float4(input.position.xyz, 1.0f), world);
    vso.position = mul(vso.position, view);
    vso.position = mul(vso.position, proj);
    
    vso.uv = input.uv;
    
	return vso;
}