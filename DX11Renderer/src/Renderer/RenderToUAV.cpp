#include "Renderer/RenderToUAV.h"


namespace Yassin
{
	void RenderToUAV::Init(unsigned int renderWidth, unsigned int renderHeight)
	{
		m_Width = renderWidth;
		m_Height = renderHeight;

		HRESULT hr;
		D3D11_TEXTURE2D_DESC tDesc = {};
		tDesc.Width = renderWidth;
		tDesc.Height = renderHeight;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = 1;
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		hr = RendererContext::GetDevice()->CreateTexture2D(&tDesc, nullptr, &m_RTT);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create render target texture", L"RenderToTexture", MB_OK);
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = tDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = RendererContext::GetDevice()->CreateShaderResourceView(m_RTT.Get(), &srvDesc, &m_SRV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create shader resource view", L"RenderToTexture", MB_OK);
			return;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = tDesc.Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		hr = RendererContext::GetDevice()->CreateUnorderedAccessView(m_RTT.Get(), &uavDesc, &m_UAV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create shader resource view", L"RenderToTexture", MB_OK);
			return;
		}
	}
}