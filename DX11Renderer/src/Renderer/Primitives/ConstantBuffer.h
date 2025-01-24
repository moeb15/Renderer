#pragma once
#include "Renderer/RendererContext.h"
#include <vector>


namespace Yassin
{
	constexpr size_t SHADOW_ATLAS_BATCH_SIZE = 64;
	constexpr unsigned int SHADOW_MAP_SIZE = 256;
	constexpr unsigned int LIGHT_BATCH_SIZE = 64;

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

	struct PointLightBatch
	{
		DirectX::XMFLOAT4 lightPosition[LIGHT_BATCH_SIZE];
		float lightRadius[LIGHT_BATCH_SIZE];
	};

	// Used for compute shader to construct clusters
	struct CameraData
	{
		DirectX::XMMATRIX invProj = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT2 screenResolution = DirectX::XMFLOAT2(1920.f, 1080.f);
		DirectX::XMFLOAT2 nfPlanes = DirectX::XMFLOAT2(0.1f, 1000.f);
		DirectX::XMUINT3 clusterDim = DirectX::XMUINT3(16, 9, 24);
		float depthRangeFactor = 1.1f;
	};

	struct WorkGroupData
	{
		DirectX::XMUINT3 NumWorkGroups;
		float padding;
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

	template<typename C>
	class ConstantBuffer
	{
	public:
		ConstantBuffer()
		{
			D3D11_BUFFER_DESC cDesc = {};
			cDesc.Usage =  D3D11_USAGE_DYNAMIC;
			cDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cDesc.ByteWidth = sizeof(C);
			cDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			RendererContext::GetDevice()->CreateBuffer(&cDesc, nullptr, &m_ConstantBuffer);
		}

		virtual void UpdateBuffer(const C& data)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mRes = {};
			hr = RendererContext::GetDeviceContext()->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);
			if(FAILED(hr))
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Failed to map to constant buffer", L"Error", MB_OK);
				return;
			}
			memcpy(mRes.pData, &data, sizeof(data));
			RendererContext::GetDeviceContext()->Unmap(m_ConstantBuffer.Get(), 0);
		}

		ID3D11Buffer* GetConstantBuffer() { return m_ConstantBuffer.Get(); }
	
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
	public:
		inline void Bind(unsigned int slot)
		{
			if(slot == 0)
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Vertex shader constant buffer at slot 0 reserved for TransformBuffer", L"Error", MB_OK);
				return;
			}
			RendererContext::GetDeviceContext()->VSSetConstantBuffers(slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
	public:
		inline void Bind(unsigned int slot)
		{
			RendererContext::GetDeviceContext()->PSSetConstantBuffers(slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class ComputeConstantBuffer : public ConstantBuffer<C>
	{
	public:
		inline void Bind(unsigned int slot)
		{
			RendererContext::GetDeviceContext()->CSSetConstantBuffers(slot, 1, m_ConstantBuffer.GetAddressOf());
		}
	};

	class TransformBuffer : public ConstantBuffer<MatrixBuffer>
	{
	public:
		inline void SetWorld(DirectX::XMMATRIX& world) { m_MatrixBuffer.world = world; }
		inline void SetView(DirectX::XMMATRIX& view) { m_MatrixBuffer.view = view; }
		inline void SetProjection(DirectX::XMMATRIX& proj) { m_MatrixBuffer.proj = proj; }
		inline void SetViewProjection(DirectX::XMMATRIX& viewProj) { m_MatrixBuffer.viewProj = viewProj; }

		inline DirectX::XMMATRIX& GetWorld() { return m_MatrixBuffer.world; }
		inline const DirectX::XMMATRIX& GetViewProjection() const { return m_MatrixBuffer.viewProj; }

		inline MatrixBuffer& GetMatrixBuffer() { return m_MatrixBuffer; }

		inline void SetTransformBuffer()
		{
			RendererContext::GetDeviceContext()->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		}

	private:
		MatrixBuffer m_MatrixBuffer;
	};

	class LightPosBuffer : public VertexConstantBuffer<LightPositionBuffer> {};

	class LightPropsBuffer : public PixelConstantBuffer<LightPropertiesBuffer> {};

	class TransparencyBuffer : public PixelConstantBuffer<BlendBuffer> {};

	class FlatColorBuffer : public PixelConstantBuffer<FlatColorBufferType> {};

	class CameraBuffer : public VertexConstantBuffer<CameraPositionType> {};

	class LightDirectionBuffer : public PixelConstantBuffer<LightDirectionType> {};

	class CameraDataBuffer : public ComputeConstantBuffer<CameraData> 
	{
	public:
		void SetInvProj(const DirectX::XMMATRIX& invProj) { m_Data.invProj = invProj; }
		void Update() { UpdateBuffer(m_Data); }

	private:
		CameraData m_Data;
	};

	class WorkGroupBuffer : public ComputeConstantBuffer<WorkGroupData>
	{
	public:
		void SetData(const WorkGroupData& data) { m_Data = data; }
		void Update() { UpdateBuffer(m_Data); }

	private:
		WorkGroupData m_Data;
	};

	class SSAOPropertiesBuffer : public PixelConstantBuffer<SSAOProperties> 
	{
	public:
		SSAOProperties& GetProperties() { return m_Props; };
		void SetProperties(const SSAOProperties& props) { m_Props = props; }

		void Update()
		{
			UpdateBuffer(m_Props);
		}

	private:
		SSAOProperties m_Props;
	};

	class ShadowAtlasBuffer : public ComputeConstantBuffer<ShadowAtlasType> 
	{
	public:
		void SetOffsets(const ShadowAtlasType& offsets)
		{
			m_Offsets = offsets;
		}

		void Update()
		{
			UpdateBuffer(m_Offsets);
		}

	private:
		ShadowAtlasType m_Offsets;
	};

	class ScreenBuffer : public PixelConstantBuffer<ScreenPropertiesBuffer> 
	{
	public:
		void SetHorizontalBlur() { m_ScreenProps.blurType = 0.0f; }
		void SetVerticalBlur() { m_ScreenProps.blurType = 1.0f; }
		void SetWidth(float width) { m_ScreenProps.screenWidth = width; }
		void SetHeight(float height) { m_ScreenProps.screenHeight = height; }
		
		void Update()
		{
			UpdateBuffer(m_ScreenProps);
		}
	private:
		ScreenPropertiesBuffer m_ScreenProps;
	};
}