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

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_ClusterBuffer.Get(), &uavDesc, &m_ClusterUAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create cluster unordered access view", L"ClusterBuffer", MB_OK);
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

		D3D11_BUFFER_DESC lightGridBuffer = {};
		lightGridBuffer.Usage = D3D11_USAGE_DEFAULT;
		lightGridBuffer.ByteWidth = sizeof(LightGrid) * NUM_CLUSTERS;
		lightGridBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		lightGridBuffer.StructureByteStride = sizeof(LightGrid);
		lightGridBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&lightGridBuffer, nullptr, &m_LightGridBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create light grid buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = NUM_CLUSTERS;

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_LightGridBuffer.Get(), &uavDesc, &m_LightGridUAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create light grid unordered access view", L"ClusterBuffer", MB_OK);
			return;
		}

		outputBuffer = {};
		outputBuffer.Usage = D3D11_USAGE_STAGING;
		outputBuffer.ByteWidth = sizeof(LightGrid) * NUM_CLUSTERS;
		outputBuffer.BindFlags = 0;
		outputBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		outputBuffer.StructureByteStride = sizeof(LightGrid);
		outputBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&outputBuffer, nullptr, &m_LightGridOutputBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create light grid output buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_BUFFER_DESC globalIdxBuffer = {};
		globalIdxBuffer.Usage = D3D11_USAGE_DEFAULT;
		globalIdxBuffer.ByteWidth = sizeof(unsigned int) * Yassin::MAX_LIGHT_COUNT * NUM_CLUSTERS;
		globalIdxBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		globalIdxBuffer.StructureByteStride = sizeof(unsigned int);
		globalIdxBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&globalIdxBuffer, nullptr, &m_GlobalLightIdxBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light index buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = Yassin::MAX_LIGHT_COUNT * NUM_CLUSTERS;

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_GlobalLightIdxBuffer.Get(), &uavDesc, &m_GlobalLightIdxUAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light index unordered access view", L"ClusterBuffer", MB_OK);
			return;
		}

		outputBuffer = {};
		outputBuffer.Usage = D3D11_USAGE_STAGING;
		outputBuffer.ByteWidth = sizeof(unsigned int) * Yassin::MAX_LIGHT_COUNT * NUM_CLUSTERS;
		outputBuffer.BindFlags = 0;
		outputBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		outputBuffer.StructureByteStride = sizeof(unsigned int);
		outputBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&outputBuffer, nullptr, &m_GlobalLightIdxOutputBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light index output buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_BUFFER_DESC globalCountBuffer = {};
		globalCountBuffer.Usage = D3D11_USAGE_DEFAULT;
		globalCountBuffer.ByteWidth = sizeof(unsigned int);
		globalCountBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		globalCountBuffer.StructureByteStride = sizeof(unsigned int);
		globalCountBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&globalCountBuffer, nullptr, &m_GlobalLightCountBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light count buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = 1;

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_GlobalLightCountBuffer.Get(), &uavDesc, &m_GlobalLightCountUAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light count unordered access view", L"ClusterBuffer", MB_OK);
			return;
		}

		outputBuffer = {};
		outputBuffer.Usage = D3D11_USAGE_STAGING;
		outputBuffer.ByteWidth = sizeof(unsigned int);
		outputBuffer.BindFlags = 0;
		outputBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		outputBuffer.StructureByteStride = sizeof(unsigned int);
		outputBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&outputBuffer, nullptr, &m_GlobalLightCountOutputBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create global light count output buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_BUFFER_DESC lightDataBuffer = {};
		lightDataBuffer.Usage = D3D11_USAGE_DYNAMIC;
		lightDataBuffer.ByteWidth = sizeof(LightCullingData) * Yassin::MAX_LIGHT_COUNT;
		lightDataBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		lightDataBuffer.StructureByteStride = sizeof(LightCullingData);
		lightDataBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightDataBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		hr = RendererContext::GetDevice()->CreateBuffer(&lightDataBuffer, nullptr, &m_LightDataBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create light data buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = Yassin::MAX_LIGHT_COUNT;

		hr = RendererContext::GetDevice()->CreateShaderResourceView(m_LightDataBuffer.Get(), &srvDesc, &m_LightDataSRV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create light data shader resource view", L"ClusterBuffer", MB_OK);
			return;
		}

		clusterBuffer = {};
		clusterBuffer.Usage = D3D11_USAGE_DYNAMIC;
		clusterBuffer.ByteWidth = sizeof(Cluster) * NUM_CLUSTERS;
		clusterBuffer.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		clusterBuffer.StructureByteStride = sizeof(Cluster);
		clusterBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		clusterBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = RendererContext::GetDevice()->CreateBuffer(&clusterBuffer, nullptr, &m_ClusterDataBuffer);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create cluster data buffer", L"ClusterBuffer", MB_OK);
			return;
		}

		srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = NUM_CLUSTERS;

		hr = RendererContext::GetDevice()->CreateShaderResourceView(m_ClusterDataBuffer.Get(), &srvDesc, &m_ClusterDataSRV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create cluster data shader resource view", L"ClusterBuffer", MB_OK);
			return;
		}

		m_LightData.reserve(Yassin::MAX_LIGHT_COUNT);
		m_ActiveLightsVec.reserve(Yassin::MAX_LIGHT_COUNT);
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
		constructClusters->SetUAV(0, m_ClusterUAV.Get());

		RendererContext::GetDeviceContext()->Dispatch(workGroupX, workGroupY, workGroupZ);

		RendererContext::GetDeviceContext()->CopyResource(m_OutputBuffer.Get(), m_ClusterBuffer.Get());
		D3D11_MAPPED_SUBRESOURCE clusterData = {};

		RendererContext::GetDeviceContext()->Map(m_OutputBuffer.Get(), 0, D3D11_MAP_READ, 0, &clusterData);

		m_Data = reinterpret_cast<Cluster*>(clusterData.pData);

		RendererContext::GetDeviceContext()->Unmap(m_OutputBuffer.Get(), 0);
	}

	void ClusterBuffer::ConstructAABB()
	{
		for(unsigned int i = 0; i < Yassin::NUM_CLUSTERS; i++)
		{
			Cluster curCluster = *(m_Data + i);
			DirectX::XMVECTOR minPoint, maxPoint;
			minPoint = DirectX::XMLoadFloat4(&curCluster.minPoint);
			maxPoint = DirectX::XMLoadFloat4(&curCluster.maxPoint);
			DirectX::BoundingBox aabb;
			DirectX::BoundingBox::CreateFromPoints(aabb, minPoint, maxPoint);

			m_AABB[i] = aabb;
		}
	}

	void Yassin::ClusterBuffer::GenerateLightLists()
	{
		D3D11_MAPPED_SUBRESOURCE mRes = {};

		RendererContext::GetDeviceContext()->Map(m_ClusterDataBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);
		memcpy(mRes.pData, m_Data, sizeof(Cluster) * NUM_CLUSTERS);
		RendererContext::GetDeviceContext()->Unmap(m_ClusterDataBuffer.Get(), 0);

		RendererContext::GetDeviceContext()->Map(m_LightDataBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);
		memcpy(mRes.pData, m_LightData.data(), sizeof(LightCullingData) * Yassin::MAX_LIGHT_COUNT);
		RendererContext::GetDeviceContext()->Unmap(m_LightDataBuffer.Get(), 0);

		unsigned int workGroupX = (unsigned int)std::ceil(CLUSTER_DIM_X / 8.f);
		unsigned int workGroupY = (unsigned int)std::ceil(CLUSTER_DIM_Y / 4.f);
		unsigned int workGroupZ = (unsigned int)std::ceil(CLUSTER_DIM_Z / 4.f);

		m_WorkGroupBuffer.SetData({ DirectX::XMUINT3(workGroupX, workGroupY, workGroupZ), 0.0f });
		m_WorkGroupBuffer.Update();
		m_CameraData.Update();

		ComputeShader* generateLightLists = ShaderLibrary::GetCompute("Generate Light Lists");

		m_CameraData.Bind(CSBufferSlot::CameraDataCB);
		generateLightLists->Bind();
		generateLightLists->SetSRV(0, m_LightDataSRV.Get());
		generateLightLists->SetSRV(1, m_ClusterDataSRV.Get());
		generateLightLists->SetUAV(0, m_GlobalLightIdxUAV.Get());
		generateLightLists->SetUAV(1, m_LightGridUAV.Get());
		generateLightLists->SetUAV(2, m_GlobalLightCountUAV.Get());

		RendererContext::GetDeviceContext()->Dispatch(workGroupX, workGroupY, workGroupZ);

		RendererContext::GetDeviceContext()->CopyResource(m_GlobalLightIdxOutputBuffer.Get(), m_GlobalLightIdxBuffer.Get());
		RendererContext::GetDeviceContext()->CopyResource(m_GlobalLightCountOutputBuffer.Get(), m_GlobalLightCountBuffer.Get());
		RendererContext::GetDeviceContext()->CopyResource(m_LightGridOutputBuffer.Get(), m_LightGridBuffer.Get());

		RendererContext::GetDeviceContext()->Map(m_GlobalLightIdxOutputBuffer.Get(), 0, D3D11_MAP_READ, 0, &mRes);

		m_ActiveLights = reinterpret_cast<unsigned int*>(mRes.pData);

		RendererContext::GetDeviceContext()->Unmap(m_GlobalLightIdxOutputBuffer.Get(), 0);

		RendererContext::GetDeviceContext()->Map(m_GlobalLightCountOutputBuffer.Get(), 0, D3D11_MAP_READ, 0, &mRes);

		m_LightCount = *reinterpret_cast<unsigned int*>(mRes.pData);

		RendererContext::GetDeviceContext()->Unmap(m_GlobalLightCountOutputBuffer.Get(), 0);

		RendererContext::GetDeviceContext()->Map(m_LightGridOutputBuffer.Get(), 0, D3D11_MAP_READ, 0, &mRes);

		m_LightGridData = reinterpret_cast<LightGrid*>(mRes.pData);

		RendererContext::GetDeviceContext()->Unmap(m_LightGridOutputBuffer.Get(), 0);
	}
}