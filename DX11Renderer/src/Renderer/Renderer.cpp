#include "Renderer/Renderer.h"
#include "Renderer/MaterialInstance.h"
#include <backends/imgui_impl_dx11.h>

namespace Yassin
{
	void Renderer::Init(int width, int height, HWND hWnd, bool fullscreen)
	{
		m_Context = std::make_unique<RendererContext>();
		m_Context->Init(width, height, hWnd, fullscreen);

		m_DepthPass = std::make_unique<RenderToTexture>();
		m_DepthPass->Init(2048, 2048, 1.f, 100.f);

		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MaterialSystem = std::make_unique<MaterialSystem>();
		m_TextureLibrary = std::make_unique<TextureLibrary>();

		TextureLibrary::Init();
		TextureLibrary::Add("Missing Texture", "src/Assets/Textures/ErrorTexture.png", TextureType::Tex2D);
		TextureLibrary::Add("Stone", "src/Assets/Textures/stone.png", TextureType::Tex2D);
		TextureLibrary::Add("Metal", "src/Assets/Textures/Metal.png", TextureType::Tex2D);

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");
		ShaderLibrary::Add("Depth Shader", L"src/Shaders/CSO/DepthVS.cso", L"src/Shaders/CSO/DepthPS.cso");
		ShaderLibrary::Add("Shadow Map Shader", L"src/Shaders/CSO/ShadowMapVS.cso", L"src/Shaders/CSO/ShadowMapPS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Error Material", ShaderLibrary::Get("Test Shader"));
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
		MaterialSystem::Add("Depth Material", ShaderLibrary::Get("Depth Shader"));
		MaterialSystem::Add("Shadow Map Material", ShaderLibrary::Get("Shadow Map Shader"));
	}
	
	void Renderer::BeginScene(float r, float g, float b, float a)
	{
		m_BackBufferColor[0] = r;
		m_BackBufferColor[1] = g;
		m_BackBufferColor[2] = b;
		m_BackBufferColor[3] = a;

		//m_Context->ClearRenderTarget(r, g, b, a);
		//m_Context->SetBackBufferRenderTarget();
	}

	void Renderer::EndScene()
	{
		m_Context->SwapBuffers(true);
	}

	void Renderer::Submit(Renderable* renderable)
	{
		if (!renderable) return;
		
		if(renderable->GetMaterialInstance()->IsIlluminated())
		{
			m_DepthRenderQueue.push(renderable);
		}

		if(renderable->GetObjectVisibility() == ObjectVisibility::Transparent)
		{
			m_TransparentRenderQueue.push(renderable);
		}
		else
		{
			m_OpaqueRenderQueue.push(renderable);
		}
	}

	void Renderer::Render(Camera& camera, DirectX::XMMATRIX& lightViewProj)
	{
		// Depth pre-pass
		DepthPrePass(lightViewProj);

		RendererContext::ClearRenderTarget(
			m_BackBufferColor[0],
			m_BackBufferColor[1],
			m_BackBufferColor[2],
			m_BackBufferColor[3]);

		while(!m_OpaqueRenderQueue.empty())
		{
			Renderable* rPtr = m_OpaqueRenderQueue.front();

			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;

			camera.GetViewMatrix(view);
			camera.GetProjectionMatrix(proj);

			DirectX::XMMATRIX viewProj =
				DirectX::XMMatrixMultiply(view, proj);

			rPtr->UpdateShadowMap(m_DepthPass->GetSRV());
			rPtr->Render(viewProj);
			rPtr->UnbindSRV();
			m_OpaqueRenderQueue.pop();
		}

		// Enable alpha blending for transparent objects
		RendererContext::EnableAlphaBlending();

		while (!m_TransparentRenderQueue.empty())
		{
			Renderable* rPtr = m_TransparentRenderQueue.front();

			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;

			camera.GetViewMatrix(view);
			camera.GetProjectionMatrix(proj);

			DirectX::XMMATRIX viewProj =
				DirectX::XMMatrixMultiply(view, proj);

			rPtr->UpdateShadowMap(m_DepthPass->GetSRV());
			rPtr->Render(viewProj);
			rPtr->UnbindSRV();
			m_TransparentRenderQueue.pop();
		}

		RendererContext::DisableAlphaBlending();
	}

	void Renderer::DepthPrePass(DirectX::XMMATRIX& lightViewProj)
	{
		m_DepthPass->SetRenderTarget();
		m_DepthPass->ClearRenderTarget(0.f, 0.f, 0.f, 1.0f);

		std::pair<VertexShader*, PixelShader*> depthShaders = ShaderLibrary::Get("Depth Shader");
		
		while(!m_DepthRenderQueue.empty())
		{
			Renderable* rPtr = m_DepthRenderQueue.front();

			rPtr->GetVertexBuffer()->Bind(0);
			rPtr->GetIndexBuffer()->Bind();

			rPtr->GetTransformBuffer()->SetViewProjection(lightViewProj);
			rPtr->GetTransformBuffer()->UpdateBuffer(rPtr->GetTransformBuffer()->GetMatrixBuffer());
			rPtr->GetTransformBuffer()->SetTransformBuffer();

			RendererContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			depthShaders.first->Bind();
			depthShaders.second->Bind();

			RendererContext::GetDeviceContext()->DrawIndexed(rPtr->GetIndexBuffer()->GetIndexCount(), 0, 0);
			m_DepthRenderQueue.pop();
		}

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}
}