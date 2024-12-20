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
		m_TextureLibrary = std::make_unique<TextureLibrary>();

		TextureLibrary::Init();
		TextureLibrary::Add("Missing Texture", "src/Assets/Textures/ErrorTexture.png", TextureType::Tex2D);
		TextureLibrary::Add("Stone", "src/Assets/Textures/stone.png", TextureType::Tex2D);

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");
		ShaderLibrary::Add("Depth Shader", L"src/Shaders/CSO/DepthVS.cso", L"src/Shaders/CSO/DepthPS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Error Material", ShaderLibrary::Get("Test Shader"));
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
		MaterialSystem::Add("Depth Material", ShaderLibrary::Get("Depth Shader"));
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
		if(renderable->GetObjectVisibility() == ObjectVisibility::Transparent)
		{
			m_TransparentRenderQueue.push(renderable);
		}
		else
		{
			m_OpaqueRenderQueue.push(renderable);
		}
	}

	void Renderer::Render(Camera& camera)
	{
		while(!m_TransparentRenderQueue.empty())
		{
			Renderable* rPtr = m_TransparentRenderQueue.front();

			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;

			camera.GetViewMatrix(view);
			camera.GetProjectionMatrix(proj);

			rPtr->Render(view, proj);
			m_TransparentRenderQueue.pop();
		}

		while(!m_OpaqueRenderQueue.empty())
		{
			Renderable* rPtr = m_OpaqueRenderQueue.front();

			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;

			camera.GetViewMatrix(view);
			camera.GetProjectionMatrix(proj);

			rPtr->Render(view, proj);
			m_OpaqueRenderQueue.pop();
		}
	}
}