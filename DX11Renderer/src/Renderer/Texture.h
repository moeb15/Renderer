#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	class Texture2D
	{
	public:
		Texture2D() = default;

		void Init(const char* textureFile);

		inline ID3D11ShaderResourceView* GetTexture() { return m_SRV.Get(); }

		void Bind(unsigned int slot)
		{
			RendererContext::GetDeviceContext()->PSSetShaderResources(slot, 1, m_SRV.GetAddressOf());
		}

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline int GetChannels() const { return m_Channels; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

		int m_Width;
		int m_Height;
		int m_Channels;
	};
}
