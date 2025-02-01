struct LightData
{
    float3 position;
    float radius;
    uint enabled;
    float3 padding;
};

struct ClusterAABB
{
    float4 minPoint;
    float4 maxPoint;
};

struct Grid
{
    uint offset;
    uint count;
};

StructuredBuffer<LightData> Lights          : register(t0);
StructuredBuffer<ClusterAABB> Clusters      : register(t1);

cbuffer CameraDataBuffer                    : register(b0)
{
    row_major matrix view;
    float2 screenRes;
    float2 nfPlanes;
    uint3 clusterDims;
    float depthRangeFactor;
};

RWStructuredBuffer<uint> GlobalLightIndex   : register(u0); // list of active lights in the scene
RWStructuredBuffer<Grid> LightGrid          : register(u1); // array containing offset and number of lights in a cluster
RWStructuredBuffer<uint> GlobalIndexCount   : register(u2); // number of active lights in the scene

groupshared LightData sharedLights[8 * 4 * 4];

bool testSphereAABB(uint light, uint clusterIndex);
float squareDistPointAABB(float3 pos, uint clusterIndex);

[numthreads(8, 4, 4)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // Reset every frame
    GlobalIndexCount[0] = 0;
    uint lightCount;
    uint stride;
    Lights.GetDimensions(lightCount, stride);
    
    uint threadCount = 8 * 4 * 4;
    uint numBatches = ceil((lightCount + threadCount - 1) / threadCount);
    uint clusterIndex = DTid.x + DTid.y * clusterDims.x + DTid.z * clusterDims.x * clusterDims.y;
    
    // local thread variables
    uint visibleLightCount = 0;
    uint visibleLightIndices[100];
    
    // traversing list of lights in batches of equal size to thread 
    // group which is equal to 4 slights of the cluster grid at once
    for (uint batch = 0; batch < numBatches; batch++)
    {
        uint lightIndex = batch * threadCount + (DTid.x + DTid.y * 8 + DTid.z * 8 * 4);
        
        // prevent overflow by clamping to last light which is always null;
        lightIndex = min(lightCount, lightIndex);
        
        // populating shared light array
        sharedLights[DTid.x + DTid.y * 8 + DTid.z * 8 * 4] = Lights[lightIndex];
        GroupMemoryBarrierWithGroupSync();
        
        // iterating within the current batch of lights
        for (uint light = 0; light < threadCount; light++)
        {
            if (sharedLights[light].enabled == 1)
            {
                if (testSphereAABB(light, clusterIndex))
                {
                    visibleLightIndices[visibleLightCount] = batch * threadCount + light;
                    visibleLightCount += 1;
                }
            }
        }

    }
    
    // we want all thread groups to have completed the light tests before continuing
    GroupMemoryBarrierWithGroupSync();
    
    // adding the light indices to the cluster light index list
    uint offset = GlobalIndexCount[0];
    InterlockedAdd(GlobalIndexCount[0], visibleLightCount);
    for (uint i = 0; i < visibleLightCount; i++)
    {
        GlobalLightIndex[offset + i] = visibleLightIndices[i];
    }

    // updating the light grid for each cluster
    LightGrid[clusterIndex].offset = offset;
    LightGrid[clusterIndex].count = visibleLightCount;
}

bool testSphereAABB(uint light, uint clusterIndex)
{
    float radius = sharedLights[light].radius;
    float4 pos = float4(sharedLights[light].position, 1.0f);
    float3 center = mul(pos, view).xyz;
    float squareDist = squareDistPointAABB(center, clusterIndex);
    
    return squareDist <= (radius * radius);
}

float squareDistPointAABB(float3 pos, uint clusterIndex)
{
    float squareDist = 0.0f;
    ClusterAABB curCluster = Clusters[clusterIndex];
    //curCluster.maxPoint[3] = clusterIndex;
    
    for (int i = 0; i < 3; i++)
    {
        float v = pos[i];
        if (v < curCluster.minPoint[i])
        {
            squareDist += (curCluster.minPoint[i] - v) * (curCluster.minPoint[i] - v);
        }
        if (v > curCluster.maxPoint[i])
        {
            squareDist += (v - curCluster.maxPoint[i]) * (v - curCluster.maxPoint[i]);
        }
    }

    return squareDist;
}