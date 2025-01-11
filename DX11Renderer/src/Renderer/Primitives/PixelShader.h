#pragma once
#include "Renderer/RendererContext.h"

namespace Yassin
{
	class PixelShader
	{
	public:
		PixelShader(wchar_t* shaderFile);
		
		inline void Bind()
		{
			RendererContext::GetDeviceContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
		}

		inline void SetTexture(unsigned int slot, ID3D11ShaderResourceView* texture)
		{
			RendererContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &texture);
		}

		inline ID3DBlob* GetBlob() const { return m_Blob.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
	};
}