cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VSIn
{
    float4 position : POSITION;
};

struct VSOut
{
    float4 position : SV_POSITION;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    vso.position = mul(float4(input.position.xyz, 1.0f), world);
    vso.position = mul(vso.position, view);
    vso.position = mul(vso.position, proj);
    
	return vso;
}