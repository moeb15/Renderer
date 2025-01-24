#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/DataTypeEnums.h"
#include "Renderer/Primitives/ConstantBuffer.h"
#include "Renderer/Resources/ShaderLibrary.h"

namespace Yassin
{
	constexpr unsigned int CLUSTER_DIM_X = 16;
	constexpr unsigned int CLUSTER_DIM_Y = 9;
	constexpr unsigned int CLUSTER_DIM_Z = 24;
	constexpr unsigned int NUM_CLUSTERS = CLUSTER_DIM_X * CLUSTER_DIM_Y * CLUSTER_DIM_Z;

	struct Cluster
	{
		DirectX::XMFLOAT4 minPoint;
		DirectX::XMFLOAT4 maxPoint;
	};

	class ClusterBuffer
	{
	public:
		void Init();
		void Execute(const DirectX::XMMATRIX& invProj);
		Cluster* GetClusterData() { return m_Data; }
		unsigned int GetNumClusters() { return NUM_CLUSTERS; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ClusterBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_OutputBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;

		CameraDataBuffer m_CameraData;
		WorkGroupBuffer m_WorkGroupBuffer;

		Cluster* m_Data;
	};
}