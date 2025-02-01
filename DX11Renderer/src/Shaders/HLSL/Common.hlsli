#ifndef ALPHATEST
    #define ALPHATEST
#endif

#ifndef PI
    #define PI 3.14159265f
#endif 

#ifndef PCF_KERNEL_SIZE
    #define PCF_KERNEL_SIZE 4
#endif

#ifndef EDGE_THRESHOLD_MIN
    #define EDGE_THRESHOLD_MIN 0.0312
#endif

#ifndef EDGE_THRESHOLD_MAX
    #define EDGE_THRESHOLD_MAX 0.125
#endif

#ifndef ITERATIONS
    #define ITERATIONS 16
#endif

#ifndef SUBPIXEL_QUALITY
    #define SUBPIXEL_QUALITY 0.75
#endif

#ifndef SHADOW_ATLAS_SIZE
    #define SHADOW_ATLAS_SIZE 2048
#endif

#ifndef MAX_LIGHTS
    #define MAX_LIGHTS 128
#endif

struct PointLight
{
    float4 colour;
    float3 position;
    float radius;
    uint enabled;
    float3 padding;
};

static float FXAA_QUALITIES[16] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 2.0f, 2.0f, 2.0f, 2.0f, 4.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f };

float RGB2Luma(float3 rgb)
{
    return dot(rgb, float3(0.299f, 0.587f, 0.114f));
}

float QUALITY(unsigned int i)
{
    return FXAA_QUALITIES[i];
}