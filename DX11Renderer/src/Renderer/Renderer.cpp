#include "Renderer/Renderer.h"
#include <backends/imgui_impl_dx11.h>

namespace Yassin
{
	void Renderer::Init(int width, int height, HWND hWnd, bool fullscreen)
	{
		m_Context = std::make_unique<RendererContext>();
		m_Context->Init(width, height, hWnd, fullscreen);

		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MaterialSystem = std::make_unique<MaterialSystem>();

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
	}
	
	void Renderer::BeginScene(float r, float g, float b, float a)
	{
		m_Context->ClearRenderTarget(r, g, b, a);
		m_Context->SetBackBufferRenderTarget();
	}

	void Renderer::SetBackBufferRenderTarget()
	{
		m_Context->SetBackBufferRenderTarget();
	}

	void Renderer::ResizeBuffer(unsigned int width, unsigned int height)
	{
		m_Context->ResizeBuffer(width, height);
	}

	void Renderer::EndScene()
	{
		m_Context->SwapBuffers(true);
	}

	void Renderer::Submit(Renderable* renderable)
	{
		if (!renderable) return;
		m_RenderQueue.push(renderable);
	}

	void Renderer::Render(Camera& camera)
	{
		while(!m_RenderQueue.empty())
		{
			Renderable* rPtr = m_RenderQueue.front();

			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;

			camera.GetViewMatrix(view);
			camera.GetProjectionMatrix(proj);

			rPtr->Render(view, proj);
			m_RenderQueue.pop();
		}
	}
}