cbuffer MatrixBuffer
{
    row_major matrix world;
    row_major matrix viewProj;
};

struct VSIn
{
    float4 position : POSITION;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 depthPos : DEPTHPOS;
};

VSOut main(VSIn input)
{
    VSOut vso;
    
    input.position.w = 1.0f;
    vso.position = mul(input.position, world);
    vso.position = mul(vso.position, viewProj);
    
    vso.depthPos = vso.position;
    
	return vso;
}