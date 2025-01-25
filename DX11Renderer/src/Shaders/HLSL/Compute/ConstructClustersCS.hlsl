cbuffer CameraDataBuffer : register(b0)
{
    row_major matrix invProj;
    float2 screenRes;
    float2 nfPlanes;
    uint3 clusterDims;
    float depthRangeFactor;
};

cbuffer WorkGroupBuffer : register(b1)
{
    uint3 NumWorkGroups;
    float padding;
};

struct Cluster
{
    float4 minPoint;
    float4 maxPoint;
};

RWStructuredBuffer<Cluster> outputClusters;

float4 screenToView(float4 screen);
float4 clipToView(float4 clip);
float3 lineIntersectionToZPlane(float3 A, float3 B, float zDistance);

[numthreads(8, 8, 8)]
void main( uint3 DTid : SV_DispatchThreadID )
{   
    // Eye position in view-space
    const float3 eyePos = float3(0.0f, 0.0f, 0.0f);
    
    float clusterSizeX = screenRes.x / clusterDims.x;
    float clusterSizeY = screenRes.y / clusterDims.y;
    float2 tileSize = float2(clusterSizeX, clusterSizeY);
    
    float2 minPointXY = DTid.xy * tileSize;
    float2 maxPointXY = float2(DTid.x + 1, DTid.y + 1) * tileSize;
    
    // calculating min and max point in screen-space
    float4 maxPoint_SS = float4(maxPointXY, 0.0f, 1.0f); // top right
    float4 minPoint_SS = float4(minPointXY, 0.0f, 1.0f); // bottom left
    
    // pass min and max to view-space
    float3 maxPoint_VS = screenToView(maxPoint_SS).xyz;
    float3 minPoint_VS = screenToView(minPoint_SS).xyz;
    
    // near and far values of the cluster in view-space
    // using equation from Tiago Sousa's DOOM 2016 Siggraph presentation
    // for calculating depth values
    float tileNear = -nfPlanes.x * pow((nfPlanes.y / nfPlanes.x), DTid.z / (float) clusterDims.z);
    float tileFar = -nfPlanes.y * pow((nfPlanes.y / nfPlanes.x), (DTid.z + 1) / (float) clusterDims.z);
    
    // finding the 4 intersection points made from each point to the cluster near/far plane
    float3 minPointNear = lineIntersectionToZPlane(eyePos, minPoint_VS, tileNear);
    float3 minPointFar = lineIntersectionToZPlane(eyePos, minPoint_VS, tileFar);
    float3 maxPointNear = lineIntersectionToZPlane(eyePos, maxPoint_VS, tileNear);
    float3 maxPointFar = lineIntersectionToZPlane(eyePos, maxPoint_VS, tileFar);
    
    float3 minPointAABB = min(min(minPointNear, minPointFar), min(maxPointNear, maxPointFar));
    float3 maxPointAABB = max(max(minPointNear, minPointFar), max(maxPointNear, maxPointFar));

    // calculate cluster index
    
    uint clusterIndex = DTid.x + DTid.y * clusterDims.x + DTid.z * clusterDims.x * clusterDims.y;
    
    if (clusterIndex < clusterDims.x * clusterDims.y * clusterDims.z)
    {
        outputClusters[clusterIndex].minPoint = float4(minPointAABB, 0.0f);
        outputClusters[clusterIndex].maxPoint = float4(maxPointAABB, 0.0f);
    }
}

float4 screenToView(float4 screen)
{
    // Convert to NDC
    float2 texCoord = float2(screen.x / screenRes.x, screen.y / screenRes.y);
    
    // Convert to clipspace;
    float4 clip = float4(texCoord.x * 2.0f - 1.0f, -texCoord.y * 2.0f - 1.0f, screen.z, screen.w);
    
    return clipToView(clip);
}

float4 clipToView(float4 clip)
{
    // view-space transform
    float4 view = mul(clip, invProj);
    
    // perpsective projection
    view /= view.w;
    
    return view;
}

// creates a line from the eye to the screenpoint, then finds its intersection
// with a z oriented plane located at the given distance to the origin
float3 lineIntersectionToZPlane(float3 A, float3 B, float zDistance)
{
    // all clusters planes are aligned in the same z direction
    float3 normal = float3(0.0f, 0.0f, 1.0f);
    
    // getting the line from the eye to the tile
    float3 AB = B - A;
    
    // computing the intersection length from the line and the plane
    float t = (zDistance - dot(normal, A)) / dot(normal, AB);
    
    return A + t * AB;
}