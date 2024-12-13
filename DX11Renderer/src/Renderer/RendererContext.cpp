#include "Renderer/RendererContext.h"
#include <backends/imgui_impl_dx11.h>

namespace Yassin
{
	RendererContext* RendererContext::s_Instance = nullptr;

	RendererContext::RendererContext()
	{
		assert(s_Instance == nullptr);
		s_Instance = this;
	}

	RendererContext::~RendererContext()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void RendererContext::Init(int width, int height, HWND hWnd, bool fullscreen)
	{
		m_HWND = hWnd;
		m_Width = width;
		m_Height = height;

		HRESULT hr;
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
		DXGI_MODE_DESC* displayModeList;
		unsigned int numModes, numerator, denominator;
		size_t strLength;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;


		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), &factory);
		if (FAILED(hr)) return;

		hr = factory->EnumAdapters(0, &adapter);
		if (FAILED(hr)) return;

		hr = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(hr)) return;

		hr = adapter->GetDesc(&adapterDesc);
		if (FAILED(hr)) return;
		m_GPUMem = (adapterDesc.DedicatedVideoMemory / 1024) / 1024;
		error = wcstombs_s(&strLength, m_GPUDesc, 128, adapterDesc.Description, 128);
		if (error != 0) return;

		UINT createDeviceFlags = 0;
#if defined(_DEBUG) || defined(DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		DXGI_SWAP_CHAIN_DESC sDesc = {};
		sDesc.OutputWindow = hWnd;
		sDesc.BufferCount = 2;
		sDesc.BufferDesc.Width = width;
		sDesc.BufferDesc.Height = height;
		sDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sDesc.BufferDesc.RefreshRate.Numerator = 0;
		sDesc.BufferDesc.RefreshRate.Denominator = 0;
		sDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sDesc.SampleDesc.Count = 1;
		sDesc.SampleDesc.Quality = 0;
		sDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sDesc.Windowed = fullscreen;
		sDesc.Flags = 0;

		D3D_FEATURE_LEVEL featureLevel;

		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sDesc,
			&m_SwapChain,
			&m_Device,
			&featureLevel,
			&m_Context);

		if (FAILED(hr)) return;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTargetBuffer = nullptr;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pTargetBuffer);
		if (FAILED(hr)) return;

		hr = m_Device->CreateRenderTargetView(pTargetBuffer.Get(), nullptr, &m_RenderTarget);
		if (FAILED(hr)) return;

		D3D11_TEXTURE2D_DESC  dDesc = {};
		dDesc.Width = width;
		dDesc.Height = height;
		dDesc.MipLevels = 1;
		dDesc.ArraySize = 1;
		dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		dDesc.Usage = D3D11_USAGE_DEFAULT;
		dDesc.SampleDesc.Count = 1;
		dDesc.SampleDesc.Quality = 0;

		hr = m_Device->CreateTexture2D(&dDesc, NULL, &m_DepthStencilBuffer);
		if (FAILED(hr)) return;

		D3D11_RASTERIZER_DESC rDesc = {};
		rDesc.AntialiasedLineEnable = false;
		rDesc.CullMode = D3D11_CULL_BACK;
		rDesc.DepthBias = 0;
		rDesc.DepthBiasClamp = 0.0f;
		rDesc.DepthClipEnable = true;
		rDesc.FillMode = D3D11_FILL_SOLID;
		rDesc.FrontCounterClockwise = false;
		rDesc.ScissorEnable = false;
		rDesc.SlopeScaledDepthBias = 0.0f;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, &m_DepthStencil);
		if (FAILED(hr)) return;

		hr = m_Device->CreateRasterizerState(&rDesc, &m_SolidRS);
		if (FAILED(hr)) return;

		rDesc.FillMode = D3D11_FILL_WIREFRAME;

		hr = m_Device->CreateRasterizerState(&rDesc, &m_WireFrameRS);

		m_Context->RSSetState(m_SolidRS.Get());

		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;

		m_Context->RSSetViewports(1, &m_Viewport);

		ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());
	}

	void RendererContext::SwapBuffers(bool vSync)
	{
		if(vSync)
		{
			m_SwapChain->Present(1, 0);
		}
		else
		{
			m_SwapChain->Present(0, 0);
		}
	}

	void RendererContext::SetBackBufferRenderTarget()
	{
		m_Context->OMSetRenderTargets(1, m_RenderTarget.GetAddressOf(), m_DepthStencil.Get());
	}

	void RendererContext::ResetViewport()
	{
		m_Context->RSSetViewports(1, &m_Viewport);
	}

	void RendererContext::ClearRenderTarget(float r, float g, float b, float a)
	{
		float colour[] = { r,g,b,a };
		m_Context->ClearRenderTargetView(m_RenderTarget.Get(), colour);
		m_Context->ClearDepthStencilView(m_DepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}
