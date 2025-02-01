#pragma once
#include <DirectXMath.h>

namespace Yassin
{
	constexpr size_t MAX_LIGHT_COUNT = 128;
	constexpr size_t SHADOW_ATLAS_BATCH_SIZE = 64;
	constexpr unsigned int SHADOW_MAP_SIZE = 256;
	constexpr unsigned int LIGHT_BATCH_SIZE = 64;

	constexpr unsigned int CLUSTER_DIM_X = 16;
	constexpr unsigned int CLUSTER_DIM_Y = 9;
	constexpr unsigned int CLUSTER_DIM_Z = 24;
	constexpr unsigned int NUM_CLUSTERS = CLUSTER_DIM_X * CLUSTER_DIM_Y * CLUSTER_DIM_Z;

	struct MatrixBuffer
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX proj = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixIdentity();
	};

	struct LightPositionBuffer
	{
		DirectX::XMMATRIX lightViewProj = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 lightPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT2 padding;
	};

	struct LightPropertiesBuffer
	{
		DirectX::XMFLOAT4 ambientColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		DirectX::XMFLOAT4 diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMFLOAT4 specularColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);;
		float specularPower = 32.f;
		float bias = 0.022f;
		DirectX::XMFLOAT2 padding;
	};

	struct PointLightData
	{
		DirectX::XMFLOAT4 colour;
		DirectX::XMFLOAT3 position;
		float radius;
		unsigned int enabled = 0;
		DirectX::XMFLOAT3 padding;
	};

	struct PointLightBatch
	{
		PointLightData data[MAX_LIGHT_COUNT];
	};

	struct BlendBuffer
	{
		float blendAmount = 1.0f;
		DirectX::XMFLOAT3 padding;
	};

	struct ScreenPropertiesBuffer
	{
		float screenWidth = 1920.f;
		float screenHeight = 1080.f;
		float blurType = 0.0f;
		float padding;
	};

	struct FlatColorBufferType
	{
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
	};

	struct CameraPositionType
	{
		DirectX::XMFLOAT3 cameraPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		float padding;
	};

	struct LightDirectionType
	{
		DirectX::XMFLOAT3 lightDir = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
		float padding;
	};

	struct ShadowAtlasType
	{
		DirectX::XMFLOAT2 offSets[SHADOW_ATLAS_BATCH_SIZE];
	};

	// Used for compute shader to construct clusters
	struct CameraData
	{
		DirectX::XMMATRIX invProj = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT2 screenResolution = DirectX::XMFLOAT2(1920.f, 1080.f);
		DirectX::XMFLOAT2 nfPlanes = DirectX::XMFLOAT2(0.1f, 1000.f);
		DirectX::XMUINT3 clusterDim = DirectX::XMUINT3(CLUSTER_DIM_X, CLUSTER_DIM_Y, CLUSTER_DIM_Z);
		float depthRangeFactor = 1.1f;
	};

	struct WorkGroupData
	{
		DirectX::XMUINT3 NumWorkGroups;
		float padding;
	};

	struct Cluster
	{
		DirectX::XMFLOAT4 minPoint;
		DirectX::XMFLOAT4 maxPoint;
	};

	struct ClusterBatch
	{
		Cluster* clusters;
	};

	struct LightCullingData
	{
		DirectX::XMFLOAT3 position;
		float lightRadius;
		unsigned int enabled;
		DirectX::XMFLOAT3 padding;
	};

	struct LightCullingBatch
	{
		LightCullingData* lights;
	};

	struct SSAOProperties
	{
		float screenWidth = 1920.f;
		float screenHeight = 1080.f;
		float randomTextureSize = 64.f;
		float sampleRadius = 1.0f;
		float ssaoScale = 1.0f;
		float ssaoBias = 0.1f;
		float ssaoIntensity = 2.0f;
		float padding;
	};
}