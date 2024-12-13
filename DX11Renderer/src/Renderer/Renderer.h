#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace Yassin
{
	class Renderer
	{
	public:
		Renderer(int width, int height, HWND hWnd, bool fullscreen = true,
			float screenNear = 0.1f, float screenFar = 1000.f);
		~Renderer();

		void BeginScene(float r, float g, float b, float a);
		void EndScene();

		void SetBackBufferRenderTarget();
		void ResetViewport();

		inline void GetGPUInfo(char* GPUName, int& memory)
		{
			strcpy_s(GPUName, 128, m_GPUDesc);
			memory = m_GPUMem;
		}

	private:
		bool m_Vsync;
		int m_GPUMem;
		char m_GPUDesc[128];

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencil;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_SolidRS;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_WireFrameRS;
		
		D3D11_VIEWPORT m_Viewport;

		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
		DirectX::XMFLOAT4X4 m_WorldMatrix;
		DirectX::XMFLOAT4X4 m_OrthoMatrix;
	};
}