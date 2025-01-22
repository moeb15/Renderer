Texture2D positionTex		: register(t0);
Texture2D normalTex			: register(t1);
Texture2D randomTex			: register(t2);
SamplerState clampSampler	: register(s0);
SamplerState wrapSampler	: register(s1);

cbuffer SsaoBuffer
{
    float screenWidth;
    float screenHeight;
    float randomTextureSize;
    float sampleRadius;
    float ssaoScale;
    float ssaoBias;
    float ssaoIntensity;
    float padding;
}

struct PSIn
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float AmbientOcclusion(float2 tex1, float2 tex2, float3 pos, float3 norm);

float4 main(PSIn input) : SV_TARGET
{
    float3 pos;
    float3 norm;
    float2 tex;
    float2 randomVec;
    float2 vecArr[4];
    float ambientOcc;
    float radius;
    float2 tex1;
    float2 tex2;
    
    pos = positionTex.Sample(clampSampler, input.uv);
    norm = normalTex.Sample(clampSampler, input.uv);
    norm = (norm * 2.0f) - 1.0f;
    norm = normalize(norm);
   
    //norm.x = (norm.x * 2.0f) - 1.0f;
    //norm.y = (-norm.y * 2.0f) - 1.0f;
    //norm.z = -norm.z;
    //norm = normalize(norm);
    
    tex.x = screenWidth / randomTextureSize;
    tex.y = screenHeight / randomTextureSize;
    tex *= input.uv;
    
    randomVec = randomTex.Sample(wrapSampler, tex).xy;
    randomVec = (randomVec * 2.0f) - 1.0f;
    randomVec = normalize(randomVec);
    
    vecArr[0] = float2(1.0f, 0.0f);
    vecArr[1] = float2(-1.0f, 0.0f);
    vecArr[2] = float2(0.0f, 1.0f);
    vecArr[3] = float2(0.0f, -1.0f);
    
    radius = sampleRadius / pos.z;
    int count = 4;
    
    ambientOcc = 0.0f;
    
    
    for (int i = 0; i < count; i++)
    {
        tex1 = reflect(vecArr[i], randomVec) * radius;
        tex2 = float2((tex1.x * 0.7f) - (tex1.y * 0.7f), (tex1.x * 0.7f) + (tex1.y * 0.7f));
        
        ambientOcc += AmbientOcclusion(input.uv, tex1 * 0.25f, pos, norm);
        ambientOcc += AmbientOcclusion(input.uv, tex2 * 0.5f, pos, norm);
        ambientOcc += AmbientOcclusion(input.uv, tex1 * 0.75f, pos, norm);
        ambientOcc += AmbientOcclusion(input.uv, tex2 * 1.0f, pos, norm);

    }
    
    ambientOcc /= ((float) count);
    ambientOcc = 1 - ambientOcc;
    ambientOcc *= ambientOcc;
    
    return float4(ambientOcc, ambientOcc, ambientOcc, 1.0f);
}

float AmbientOcclusion(float2 tex1, float2 tex2, float3 pos, float3 norm)
{
    float3 posVec;
    float3 vec;
    float dist;
    float occlusion;
    
    posVec = positionTex.Sample(clampSampler, tex1 + tex2);
    //posVec -= pos;
    vec = normalize(posVec);
    
    dist = length(posVec) * ssaoBias;
    occlusion = max(0.0f, (dot(norm, vec) - ssaoBias) * (1.0f / (1.0f + dist))) * ssaoIntensity;
    
    return occlusion;
}