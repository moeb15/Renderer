#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/DataTypeEnums.h"

namespace Yassin
{
	class RenderToTexture
	{
	public:
		void Init(unsigned int renderWidth, unsigned int renderHeight,
			float sNear, float sFar, RenderTargetType type);

		void SetRenderTarget();
		void ClearRenderTarget(float r, float g, float b, float a);

		inline ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }

		inline void GetPerspectiveMatrix(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_ProjMatrix); }
		inline void GetOrthoMatrix(DirectX::XMMATRIX& matrix) const { matrix = DirectX::XMLoadFloat4x4(&m_OrthoMatrix); }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

	private:
		unsigned int m_Width;
		unsigned int m_Height;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RTT;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DSB;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
		D3D11_VIEWPORT m_VP;

		DirectX::XMFLOAT4X4 m_ProjMatrix;
		DirectX::XMFLOAT4X4 m_OrthoMatrix;

		RenderTargetType m_Type;
		unsigned int m_ClearFlags;
	};
}

