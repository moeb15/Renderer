#include "Renderer/Renderer.h"
#include "Renderer/Resources/MaterialInstance.h"
#include <backends/imgui_impl_dx11.h>

namespace Yassin
{
	void Renderer::Init(int width, int height, HWND hWnd, bool fullscreen)
	{

		m_Context = std::make_unique<RendererContext>();
		m_Context->Init(width, height, hWnd, fullscreen);

		m_SceneTexture = std::make_unique<RenderToTexture>();
		m_SceneTexture->Init(width, height, 0.1f, 1000.f, RenderTargetType::DepthStencil);

		m_DepthPass = std::make_unique<RenderToTexture>();
		m_DepthPass->Init(2048, 2048, 1.f, 100.f, RenderTargetType::DepthMap);

		m_GBuffer = std::make_unique<GBuffer>();
		m_GBuffer->Init(width, height, 0.1f, 1000.f);

		m_PostProcessingEnabled = false;
		m_DeferredRenderingEnabled = false;
		m_BoundingVolumesEnabled = false;
		m_BoxBlurEnabled = false;
		m_GaussianBlurEnabled = false;

		m_PostProcessSampler.Init(FilterType::Anisotropic, AddressType::Clamp);
		m_GBufferSampler.Init(FilterType::Bilinear, AddressType::Wrap);

		m_FullScreenWindow.Init(width, height);

		m_BoxBlurEffect.Init(width / 2, height / 2, 0.1f, 1000.f, width, height, BlurType::BoxBlur);
		m_GaussianBlurEffect.Init(width / 2, height / 2, 0.1f, 1000.f, width, height, BlurType::GaussianBlur);

		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MaterialSystem = std::make_unique<MaterialSystem>();
		m_TextureLibrary = std::make_unique<TextureLibrary>();

		TextureLibrary::Init();
		TextureLibrary::Add("Missing Texture", "src/Assets/Textures/ErrorTexture.png", TextureType::Tex2D);
		TextureLibrary::Add("Stone", "src/Assets/Textures/stone.png", TextureType::Tex2D);
		TextureLibrary::Add("Metal", "src/Assets/Textures/Metal.png", TextureType::Tex2D);
		TextureLibrary::Add("BoundingVolume", "src/Assets/Textures/BoundingVolumeTexture.png", TextureType::Tex2D);

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");
		ShaderLibrary::Add("Unlit Texture Shader", L"src/Shaders/CSO/UnlitTextureVS.cso", L"src/Shaders/CSO/UnlitTexturePS.cso");
		ShaderLibrary::Add("Bounding Volume Shader", L"src/Shaders/CSO/BoundingVolumeVS.cso", L"src/Shaders/CSO/BoundingVolumePS.cso");

		ShaderLibrary::Add("Shadow Map Shader", L"src/Shaders/CSO/ShadowMapVS.cso", L"src/Shaders/CSO/ShadowMapPS.cso");
		ShaderLibrary::Add("Phong Shader", L"src/Shaders/CSO/PhongDirVS.cso", L"src/Shaders/CSO/PhongDirPS.cso");
		ShaderLibrary::Add("PBR Shader", L"src/Shaders/CSO/PBR_VS.cso", L"src/Shaders/CSO/PBR_PS.cso");

		ShaderLibrary::Add("GBuffer Shader", L"src/Shaders/CSO/DeferredVS.cso", L"src/Shaders/CSO/DeferredPS.cso");
		ShaderLibrary::Add("Depth Shader", L"src/Shaders/CSO/DepthVS.cso", L"src/Shaders/CSO/DepthPS.cso");
		ShaderLibrary::Add("Gaussian Blur Shader", L"src/Shaders/CSO/PostProcessVS.cso", L"src/Shaders/CSO/GaussianBlurPS.cso");
		ShaderLibrary::Add("Box Blur Shader", L"src/Shaders/CSO/PostProcessVS.cso", L"src/Shaders/CSO/BoxBlurPS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Error Material", ShaderLibrary::Get("Test Shader"));
		MaterialSystem::Add("Flat Material", ShaderLibrary::Get("Unlit Texture Shader"));
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
		MaterialSystem::Add("Depth Material", ShaderLibrary::Get("Depth Shader"));
		MaterialSystem::Add("Shadow Map Material", ShaderLibrary::Get("Shadow Map Shader"));
		MaterialSystem::Add("Phong Material", ShaderLibrary::Get("Phong Shader"));
		MaterialSystem::Add("PBR Material", ShaderLibrary::Get("PBR Shader"));
	}
	
	void Renderer::BeginScene(float r, float g, float b, float a)
	{
		m_BackBufferColor[0] = r;
		m_BackBufferColor[1] = g;
		m_BackBufferColor[2] = b;
		m_BackBufferColor[3] = a;
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
			if(std::find(m_Renderables.begin(), m_Renderables.end(), renderable) == m_Renderables.end())
				m_Renderables.push_back(renderable);
		}
		else
		{
			m_OpaqueRenderQueue.push(renderable);
			m_GBufferQueue.push(renderable);
			if (std::find(m_Renderables.begin(), m_Renderables.end(), renderable) == m_Renderables.end())
				m_Renderables.push_front(renderable);
		}
	}

	void Renderer::Render(Camera& camera, DirectX::XMMATRIX& lightViewProj)
	{
		// Depth pre-pass
		DepthPrePass(camera, lightViewProj);
		
		//GBufferPass(camera);

		RendererContext::ClearRenderTarget(
			m_BackBufferColor[0],
			m_BackBufferColor[1],
			m_BackBufferColor[2],
			m_BackBufferColor[3]);


		if(m_DeferredRenderingEnabled)
		{
			// Testing for now
			GBufferPass(camera);
		}

		if (m_PostProcessingEnabled)
		{
			RenderSceneToTexture(camera);
			PostProcessedScene(camera);

			return;
		}

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		DirectX::XMMATRIX viewProj =
			DirectX::XMMatrixMultiply(view, proj);

		while(!m_OpaqueRenderQueue.empty())
		{
			Renderable* rPtr = m_OpaqueRenderQueue.front();

			rPtr->GetMaterialInstance()->SetShadowMap(m_DepthPass->GetSRV());
			rPtr->Render(camera, viewProj, false, m_BoundingVolumesEnabled);
			rPtr->GetMaterialInstance()->UnbindShaderResources();
			m_OpaqueRenderQueue.pop();
		}

		// Enable alpha blending for transparent objects
		RendererContext::EnableAlphaBlending();

		while (!m_TransparentRenderQueue.empty())
		{
			Renderable* rPtr = m_TransparentRenderQueue.front();

			rPtr->GetMaterialInstance()->SetShadowMap(m_DepthPass->GetSRV());
			rPtr->Render(camera, viewProj, false, m_BoundingVolumesEnabled);
			rPtr->GetMaterialInstance()->UnbindShaderResources();
			m_TransparentRenderQueue.pop();
		}

		RendererContext::DisableAlphaBlending();
	}

	void Renderer::DepthPrePass(Camera& camera, DirectX::XMMATRIX& lightViewProj)
	{
		m_DepthPass->SetRenderTarget();
		m_DepthPass->ClearRenderTarget(0.f, 0.f, 0.f, 1.0f);

		std::pair<VertexShader*, PixelShader*> depthShaders = ShaderLibrary::Get("Depth Shader");
		
		while(!m_DepthRenderQueue.empty())
		{
			Renderable* rPtr = m_DepthRenderQueue.front();

			depthShaders.first->Bind();
			depthShaders.second->Bind();

			rPtr->Render(camera, lightViewProj, true);
			
			m_DepthRenderQueue.pop();
		}

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Renderer::GBufferPass(Camera& camera)
	{
		m_GBuffer->SetRenderTargets();
		m_GBuffer->ClearRenderTargets(0.0f, 0.0f, 0.0f, 1.0f);

		std::pair<VertexShader*, PixelShader*> gBufferShaders = ShaderLibrary::Get("GBuffer Shader");

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		DirectX::XMMATRIX viewProj =
			DirectX::XMMatrixMultiply(view, proj);

		while (!m_GBufferQueue.empty())
		{
			Renderable* rPtr = m_GBufferQueue.front();

			gBufferShaders.second->SetTexture(TextureSlot::BaseTexture,
				rPtr->GetMaterialInstance()->GetTexture(TextureSlot::BaseTexture));
			m_GBufferSampler.Bind(SamplerSlot::WrapSampler);
			gBufferShaders.first->Bind();
			gBufferShaders.second->Bind();

			rPtr->Render(camera, viewProj, true);

			m_GBufferQueue.pop();
		}

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Renderer::RenderSceneToTexture(Camera& camera)
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		DirectX::XMMATRIX viewProj =
			DirectX::XMMatrixMultiply(view, proj);

		m_SceneTexture->SetRenderTarget();
		m_SceneTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		RendererContext::EnableAlphaBlending();
		
		for(int i = 0; i < m_Renderables.size(); i++)
		{
			Renderable* rPtr = m_Renderables[i];

			rPtr->GetMaterialInstance()->SetShadowMap(m_DepthPass->GetSRV());
			rPtr->Render(camera, viewProj);
			rPtr->GetMaterialInstance()->UnbindShaderResources();
		}

		RendererContext::DisableAlphaBlending();

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Renderer::PostProcessedScene(Camera& camera)
	{
		RendererContext::DisableZBuffer();

		if(m_BoxBlurEnabled) m_BoxBlurEffect.BlurScene(camera, m_SceneTexture.get());
		if(m_GaussianBlurEnabled) m_GaussianBlurEffect.BlurScene(camera, m_SceneTexture.get());


		std::pair<VertexShader*, PixelShader*> textureShader = ShaderLibrary::Get("Unlit Texture Shader");

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMMATRIX viewProj;

		camera.GetDefaultView(view);
		m_SceneTexture->GetOrthoMatrix(proj);

		viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_FullScreenWindow.Render(viewProj);
		textureShader.second->SetTexture(0, m_SceneTexture->GetSRV());
		m_PostProcessSampler.Bind(0);
		textureShader.first->Bind();
		textureShader.second->Bind();
		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		RendererContext::EnableZBuffer();
	}
}