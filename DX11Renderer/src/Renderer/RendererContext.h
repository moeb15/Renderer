#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace Yassin
{
	class RendererContext
	{
	public:
		RendererContext();
		~RendererContext();
		void Init(int width, int height, HWND hWnd, bool fullscreen = true);
		void SwapBuffers(bool vSync);

		static ID3D11Device* GetDevice() { return s_Instance->m_Device.Get(); }
		static ID3D11DeviceContext* GetDeviceContext() { return s_Instance->m_Context.Get(); }
		static HWND GetWindowHandle() { return s_Instance->m_HWND; }

		static void SetBackBufferRenderTarget();
		static void ResetViewport();
		void ResizeBuffer(unsigned int width, unsigned int height);

		void ClearRenderTarget(float r, float g, float b, float a);

		static void GetGPUInfo(char* GPUName, int& memory)
		{
			strcpy_s(GPUName, 128, s_Instance->m_GPUDesc);
			memory = s_Instance->m_GPUMem;
		}

	private:
		bool m_Vsync;
		int m_GPUMem;
		char m_GPUDesc[128];
		HWND m_HWND;
		int m_Width;
		int m_Height;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_SolidRS;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_WireFrameRS;

		D3D11_VIEWPORT m_Viewport;

	private:
		static RendererContext* s_Instance;
	};
}