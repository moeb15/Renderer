#include "Renderer/Utils/Cluster.h"
#include "Cluster.h"

namespace Yassin
{
	void Yassin::ClusterBuffer::Init()
	{
		HRESULT hr;

		D3D11_BUFFER_DESC clusterBuffer = {};
		clusterBuffer.Usage = D3D11_USAGE_DEFAULT;
		clusterBuffer.ByteWidth = sizeof(Cluster) * NUM_CLUSTERS;
		clusterBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		clusterBuffer.StructureByteStride = sizeof(Cluster);
		clusterBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&clusterBuffer, nullptr, &m_ClusterBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create cluster buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = NUM_CLUSTERS;

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_ClusterBuffer.Get(), &uavDesc, &m_UAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create unordered access view", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_BUFFER_DESC outputBuffer = {};
		outputBuffer.Usage = D3D11_USAGE_STAGING;
		outputBuffer.ByteWidth = sizeof(Cluster) * NUM_CLUSTERS;
		outputBuffer.BindFlags = 0;
		outputBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		outputBuffer.StructureByteStride = sizeof(Cluster);
		outputBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&outputBuffer, nullptr, &m_OutputBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create output buffer", L"ClusterBuffer", MB_OK);
			return;
		}

	}

	void ClusterBuffer::Execute(const DirectX::XMMATRIX& invProj)
	{
		unsigned int workGroupX = (unsigned int) std::ceil(CLUSTER_DIM_X / 8.f);
		unsigned int workGroupY = (unsigned int) std::ceil(CLUSTER_DIM_Y / 8.f);
		unsigned int workGroupZ = (unsigned int) std::ceil(CLUSTER_DIM_Z / 8.f);

		m_CameraData.SetInvProj(invProj);
		m_CameraData.Update();

		m_WorkGroupBuffer.SetData({ DirectX::XMUINT3(workGroupX, workGroupY, workGroupZ), 0.0f });
		m_WorkGroupBuffer.Update();

		ComputeShader* constructClusters = ShaderLibrary::GetCompute("Construct Clusters");

		m_CameraData.Bind(CSBufferSlot::CameraDataCB);
		m_WorkGroupBuffer.Bind(CSBufferSlot::WorkGroupDataCB);
		constructClusters->Bind();
		constructClusters->SetUAV(0, m_UAV.Get());

		RendererContext::GetDeviceContext()->Dispatch(workGroupX, workGroupY, workGroupZ);

		RendererContext::GetDeviceContext()->CopyResource(m_OutputBuffer.Get(), m_ClusterBuffer.Get());
		D3D11_MAPPED_SUBRESOURCE clusterData = {};

		RendererContext::GetDeviceContext()->Map(m_OutputBuffer.Get(), 0, D3D11_MAP_READ, 0, &clusterData);

		m_Data = reinterpret_cast<Cluster*>(clusterData.pData);

		RendererContext::GetDeviceContext()->Unmap(m_OutputBuffer.Get(), 0);
	}
}