#include "Renderer/Primitives/Texture.h"
#include <DirectXTex.h>

namespace Yassin
{
	bool Texture2D::Init(const char* textureFile)
	{
		HRESULT hr;
		size_t len = 128;
		wchar_t wTexFile[128];
		mbstowcs_s(&len, wTexFile, textureFile, len);
		DirectX::ScratchImage imageData;

		hr = DirectX::LoadFromWICFile(wTexFile, DirectX::WIC_FLAGS_NONE, nullptr, imageData);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Invalid texture file", wTexFile, MB_OK);

			return false;
		}

		m_Height = imageData.GetImage(0, 0, 0)->height;
		m_Width = imageData.GetImage(0, 0, 0)->width;

		D3D11_TEXTURE2D_DESC tDesc = {};
		tDesc.Height = m_Height;
		tDesc.Width = m_Width;
		tDesc.MipLevels = 0;
		tDesc.ArraySize = 1;
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA tData = {};
		tData.pSysMem = imageData.GetImage(0,0,0)->pixels;
		tData.SysMemPitch = imageData.GetImage(0,0,0)->rowPitch;
		tData.SysMemSlicePitch = imageData.GetImage(0, 0, 0)->slicePitch;

		hr = RendererContext::GetDevice()->CreateTexture2D(&tDesc, nullptr, &m_Texture);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create texture", L"Texture2D", MB_OK);
			return false;
		}

		RendererContext::GetDeviceContext()->UpdateSubresource(m_Texture.Get(), 0, nullptr, imageData.GetImage(0,0,0)->pixels, imageData.GetImage(0,0,0)->rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = tDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = -1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = RendererContext::GetDevice()->CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_SRV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create shader resource view", L"Texture2D", MB_OK);
			return false;
		}

		RendererContext::GetDeviceContext()->GenerateMips(m_SRV.Get());

		return true;
	}
}