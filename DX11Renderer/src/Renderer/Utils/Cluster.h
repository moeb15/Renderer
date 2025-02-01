#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/DataTypeEnums.h"
#include "Renderer/Primitives/CBufferDataTypes.h"
#include "Renderer/Resources/ShaderLibrary.h"
#include <DirectXCollision.h>

namespace Yassin
{
	struct LightGrid
	{
		unsigned int offset;
		unsigned int count;
	};
	
	class ClusterBuffer
	{
	public:
		void Init();
		void Execute(const DirectX::XMMATRIX& invProj);
		void ConstructAABB();
		void GenerateLightLists();
		Cluster* GetClusterData() { return m_Data; }
		DirectX::BoundingBox* GetAABB() { return m_AABB; }
		void SetLightData(std::vector<LightCullingData>& lightData) { m_LightData = lightData; }
		void SetView(const DirectX::XMMATRIX& view) { m_CameraData.SetInvProj(view); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ClusterBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_OutputBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_ClusterUAV;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightGridBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightGridOutputBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_LightGridUAV;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_GlobalLightIdxBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_GlobalLightIdxOutputBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_GlobalLightIdxUAV;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_GlobalLightCountBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_GlobalLightCountOutputBuffer;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_GlobalLightCountUAV;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightDataBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_LightDataSRV;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ClusterDataBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ClusterDataSRV;
		
		CameraDataBuffer m_CameraData;
		WorkGroupBuffer m_WorkGroupBuffer;

		Cluster* m_Data;
		DirectX::BoundingBox m_AABB[NUM_CLUSTERS];
		bool m_ActiveClusters[NUM_CLUSTERS];
		std::vector<LightCullingData> m_LightData;
		LightGrid* m_LightGridData;
		unsigned int* m_ActiveLights;
		unsigned int m_LightCount;
		std::vector<unsigned int> m_ActiveLightsVec;
	};
}