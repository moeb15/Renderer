#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/DataTypeEnums.h"

namespace Yassin
{
	class RenderToUAV
	{
	public:
		void Init(unsigned int renderWidth, unsigned int renderHeight);

		inline ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }
		inline ID3D11UnorderedAccessView* GetUAV() { return m_UAV.Get(); }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

	private:
		unsigned int m_Width;
		unsigned int m_Height;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RTT;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
	};
}

