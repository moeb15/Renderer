#include "Renderer/Primitives/TextureArray.h"

namespace Yassin
{
	bool TextureArray::Init(unsigned int numTextures, unsigned int texHeight, unsigned int texWidth)
	{
		m_Height = texHeight;
		m_Width = texWidth;
		m_NumTextures = numTextures;

		HRESULT hr;

		D3D11_TEXTURE2D_DESC tDesc = {};
		tDesc.Height = m_Height;
		tDesc.Width = m_Width;
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = numTextures;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		hr = RendererContext::GetDevice()->CreateTexture2D(&tDesc, nullptr, &m_Texture);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create texture", L"TextureArray", MB_OK);
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = tDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = numTextures;

		hr = RendererContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create shader resource view", L"Texture2D", MB_OK);
			return false;
		}

		return true;
	}

	void TextureArray::UpdateTextureArray(ID3D11Texture2D* const* textures)
	{
		for(unsigned int i = 0; i < m_NumTextures; i++)
		{
			if (!textures[i]) continue;
			RendererContext::GetDeviceContext()->CopySubresourceRegion(m_Texture.Get(), D3D11CalcSubresource(0, i, 1),
				0, 0, 0, textures[i], 0, nullptr);
		}
	}
}
