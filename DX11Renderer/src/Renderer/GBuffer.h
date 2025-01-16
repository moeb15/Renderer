#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/DataTypeEnums.h"


namespace Yassin
{
	enum Buffer 
	{
		Color,
		Normal,
		Depth,
		BUFFER_COUNT
	};

	class GBuffer
	{
	public:
		void Init(unsigned int renderWidth, unsigned int renderHeight, float sNear, float sFar);
		
		void SetRenderTargets();
		void ClearRenderTargets(float r, float g, float b, float a);

		inline ID3D11ShaderResourceView* GetSRV(Buffer type) { return m_SRV[type].Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RTT[BUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV[BUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV[BUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DSB;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
		D3D11_VIEWPORT m_VP;

		unsigned int m_Width;
		unsigned int m_Height;
	};
}