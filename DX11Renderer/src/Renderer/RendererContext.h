#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

namespace Yassin
{
	class RendererContext
	{
	public:
		RendererContext();
		~RendererContext();
		void Init(int width, int height, HWND hWnd, bool fullscreen = true, unsigned int numCPUCores = 1);
		void SwapBuffers(bool vSync);

		static ID3D11Device* GetDevice() { return s_Instance->m_Device.Get(); }
		static ID3D11DeviceContext* GetDeviceContext() { return s_Instance->m_Context.Get(); }
		static ID3D11DeviceContext* GetDeferredContext(unsigned int context = 0);
		static HWND GetWindowHandle() { return s_Instance->m_HWND; }
		static ID3D11Debug* GetDebug() { return s_Instance->m_Debug.Get(); }

		static void SetBackBufferRenderTarget(bool ImmediateContext = true);
		static void ResetViewport(bool ImmediateContext = true);
		void ResizeBuffer(unsigned int width, unsigned int height);

		static void ClearRenderTarget(float r, float g, float b, float a, bool ImmediateContext = true);
		static void ClearDepthStencil(bool ImmediateContext = true);

		static void GetGPUInfo(char* GPUName, size_t& memory)
		{
			strcpy_s(GPUName, 128, s_Instance->m_GPUDesc);
			memory = s_Instance->m_GPUMem;
		}

		static unsigned int GetWidth() { return (unsigned int)s_Instance->m_Width; }
		static unsigned int GetHeight() { return (unsigned int)s_Instance->m_Height; }

		static void EnableDepthWrites(bool ImmediateContext = true)
		{
			if (ImmediateContext)
				s_Instance->m_Context->OMSetDepthStencilState(s_Instance->m_DepthStencilState.Get(), 1);
			else
				s_Instance->GetDeferredContext(0)->OMSetDepthStencilState(s_Instance->m_DepthStencilState.Get(), 1);
		}

		static void DisableDepthWrites(bool ImmediateContext = true)
		{
			if (ImmediateContext)
				s_Instance->m_Context->OMSetDepthStencilState(s_Instance->m_PostZPassState.Get(), 1);
			else
				s_Instance->GetDeferredContext(0)->OMSetDepthStencilState(s_Instance->m_PostZPassState.Get(), 1);
		}

		static void EnableSolidRS() 
		{
			s_Instance->m_Context->RSSetState(s_Instance->m_SolidRS.Get());
		}

		static void EnableWireframeRS()
		{
			s_Instance->m_Context->RSSetState(s_Instance->m_WireFrameRS.Get());
		}

		static void ToggleRasterizerState()
		{
			if(s_Instance->m_IsSolidRaster)
				s_Instance->m_Context->RSSetState(s_Instance->m_WireFrameRS.Get());
			else
				s_Instance->m_Context->RSSetState(s_Instance->m_SolidRS.Get());
			
			s_Instance->m_IsSolidRaster = !s_Instance->m_IsSolidRaster;
		}

		static void EnableAlphaBlending()
		{
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			s_Instance->m_Context->OMSetBlendState(s_Instance->m_AlphaBlendEnable.Get(), blendFactor, 0xffffffff);
		}

		static void DisableAlphaBlending()
		{
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			s_Instance->m_Context->OMSetBlendState(s_Instance->m_AlphaBlendDisable.Get(), blendFactor, 0xffffffff);
		}

		static void EnableZBuffer()
		{
			s_Instance->m_Context->OMSetDepthStencilState(s_Instance->m_DepthStencilState.Get(), 0);
		}

		static void DisableZBuffer()
		{
			s_Instance->m_Context->OMSetDepthStencilState(s_Instance->m_2DState.Get(), 0);
		}

	private:
		bool m_Vsync;
		size_t m_GPUMem;
		char m_GPUDesc[128];
		HWND m_HWND;
		int m_Width;
		int m_Height;
		unsigned int m_NumCPUCores;
		bool m_IsSolidRaster = true;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_PostZPassState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_2DState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_SolidRS;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_WireFrameRS;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_AlphaBlendEnable;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_AlphaBlendDisable;

		std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext>> m_DeferredContexts;
		std::vector<Microsoft::WRL::ComPtr<ID3D11CommandList>> m_CommandLists;
		Microsoft::WRL::ComPtr<ID3D11Debug> m_Debug;
		D3D11_VIEWPORT m_Viewport;

	private:
		static RendererContext* s_Instance;
	};
}