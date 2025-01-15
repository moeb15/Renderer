#include "Renderer/GBuffer.h"


namespace Yassin
{
	void GBuffer::Init(unsigned int renderWidth, unsigned int renderHeight, float sNear, float sFar, RenderTargetType type)
	{
		m_Width = renderWidth;
		m_Height = renderHeight;

		HRESULT hr;
		D3D11_TEXTURE2D_DESC tDesc = {};
		tDesc.Width = renderWidth;
		tDesc.Height = renderHeight;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = 1;
		tDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		for(int i = 0; i < BUFFER_COUNT; i++)
		{
			hr = RendererContext::GetDevice()->CreateTexture2D(&tDesc, nullptr, &m_RTT[i]);
			if (FAILED(hr))
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Failed to create render target texture", L"GBuffer", MB_OK);
				return;
			}
		}

		D3D11_RENDER_TARGET_VIEW_DESC rDesc = {};
		rDesc.Format = tDesc.Format;
		rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rDesc.Texture2D.MipSlice = 0;

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			hr = RendererContext::GetDevice()->CreateRenderTargetView(m_RTT[i].Get(), &rDesc, &m_RTV[i]);
			if (FAILED(hr))
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Failed to create render target view", L"GBuffer", MB_OK);
				return;
			}
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = tDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			hr = RendererContext::GetDevice()->CreateShaderResourceView(m_RTT[i].Get(), &srvDesc, &m_SRV[i]);
			if (FAILED(hr))
			{
				MessageBox(RendererContext::GetWindowHandle(), L"Failed to create shader resource view", L"GBuffer", MB_OK);
				return;
			}
		}

		D3D11_TEXTURE2D_DESC dDesc = {};
		dDesc.Width = m_Width;
		dDesc.Height = m_Height;
		dDesc.MipLevels = 1;
		dDesc.ArraySize = 1;
		dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dDesc.SampleDesc.Count = 1;
		dDesc.SampleDesc.Quality = 0;
		dDesc.Usage = D3D11_USAGE_DEFAULT;
		dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = RendererContext::GetDevice()->CreateTexture2D(&dDesc, nullptr, &m_DSB);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create depth stencil buffer", L"GBuffer", MB_OK);
			return;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = RendererContext::GetDevice()->CreateDepthStencilView(m_DSB.Get(), &dsvDesc, &m_DSV);
		if (FAILED(hr))
		{
			MessageBox(RendererContext::GetWindowHandle(), L"Failed to create depth stencil view", L"GBuffer", MB_OK);
			return;
		}

		m_VP.Width = (float)renderWidth;
		m_VP.Height = (float)renderHeight;
		m_VP.MinDepth = 0.0f;
		m_VP.MaxDepth = 1.0f;
		m_VP.TopLeftX = 0;
		m_VP.TopLeftY = 0;
	}

	void GBuffer::SetRenderTargets()
	{
		RendererContext::GetDeviceContext()->OMSetRenderTargets(Buffer::BUFFER_COUNT, m_RTV->GetAddressOf(), m_DSV.Get());
		RendererContext::GetDeviceContext()->RSSetViewports(1, &m_VP);
	}

	void GBuffer::ClearRenderTargets(float r, float g, float b, float a)
	{
		float color[] = { r,g,b,a };

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			RendererContext::GetDeviceContext()->ClearRenderTargetView(m_RTV[i].Get(), color);
		}

		RendererContext::GetDeviceContext()->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}