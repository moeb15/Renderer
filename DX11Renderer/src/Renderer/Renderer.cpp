#include "Renderer/Renderer.h"
#include <backends/imgui_impl_dx11.h>

namespace Yassin
{
	void Renderer::Init(int width, int height, HWND hWnd, bool fullscreen)
	{
		m_Context = std::make_unique<RendererContext>();
		m_Context->Init(width, height, hWnd, fullscreen);
	}
	
	void Renderer::BeginScene(float r, float g, float b, float a)
	{
		m_Context->ClearRenderTarget(r, g, b, a);
	}

	void Renderer::EndScene()
	{
		m_Context->SwapBuffers(true);
	}
}