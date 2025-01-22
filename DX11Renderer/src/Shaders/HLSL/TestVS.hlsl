cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix proj;
    row_major matrix viewProj;
};

struct VSIn
{
    float4 position : POSITION;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    vso.position = mul(float4(input.position.xyz, 1.0f), world);
    vso.position = mul(vso.position, viewProj);
    
    vso.color = input.position;
    vso.color.w = 1.0f;
    
	return vso;
}