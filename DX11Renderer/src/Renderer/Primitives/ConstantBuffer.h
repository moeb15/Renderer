#pragma once
#include "Renderer/RendererContext.h"
#include <vector>

namespace Yassin
{
	struct MatrixBuffer
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixIdentity();
	};

	struct LightPositionBuffer
	{
		DirectX::XMMATRIX lightViewProj = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 lightPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		float padding;
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

	class TransformBuffer : public ConstantBuffer<MatrixBuffer>
	{
	public:
		inline void SetWorld(DirectX::XMMATRIX& world) { m_MatrixBuffer.world = world; }
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

	class ScreenBuffer : public PixelConstantBuffer<ScreenPropertiesBuffer> 
	{
	public:
		void SetHorizontalBlur() { m_ScreenProps.blurType = 0.0f; }
		void SetVerticalBlur() { m_ScreenProps.blurType = 1.0f; }
		
		void Update()
		{
			UpdateBuffer(m_ScreenProps);
		}
	private:
		ScreenPropertiesBuffer m_ScreenProps;
	};
}