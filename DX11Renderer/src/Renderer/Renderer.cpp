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
		m_DepthPass->Init(width, height, 1.f, 100.f, RenderTargetType::DepthMap);

		m_ShadowMap = std::make_unique<RenderToTexture>();
		m_ShadowMap->Init(2048, 2048, 1.f, 100.f, RenderTargetType::DepthMap);

		m_SoftShadow = std::make_unique<RenderToTexture>();
		m_SoftShadow->Init(width, height, 1.f, 100.f, RenderTargetType::DepthMap);

		m_GBuffer = std::make_unique<GBuffer>();
		m_GBuffer->Init(width, height, 0.1f, 1000.f);

		m_UpdateDebug = 165;
		m_CurrentDebugFrame = 0;
		
		m_ShadowAtlas = std::make_unique<RenderToUAV>();
		m_ShadowAtlas->Init(2048, 2048);

		m_Ambient = nullptr;

		unsigned int lightDepthX = SHADOW_MAP_SIZE;
		unsigned int lightDepthY = SHADOW_MAP_SIZE;
		unsigned int numLightMaps = (m_ShadowAtlas->GetHeight() * m_ShadowAtlas->GetWidth()) / (lightDepthX * lightDepthY);
		for(unsigned int i = 0 ; i < numLightMaps; i++)
		{
			std::unique_ptr<RenderToTexture> lDepthMap = std::make_unique<RenderToTexture>();
			lDepthMap->Init(lightDepthX, lightDepthY, 1.f, 100.f, RenderTargetType::DepthMap);
			m_LightDepthMaps.push_back(std::move(lDepthMap));
		}

		unsigned int numLightBuffers = numLightMaps / SHADOW_ATLAS_BATCH_SIZE;
		for(unsigned int i = 0 ; i < numLightBuffers; i++)
		{
			std::unique_ptr<ShadowAtlasBuffer> buffer = std::make_unique<ShadowAtlasBuffer>();
			m_ShadowAtlasBuffers.push_back(std::move(buffer));
		}

		m_PostProcessingEnabled = false;
		m_DeferredRenderingEnabled = false;
		m_BoundingVolumesEnabled = false;
		m_BoxBlurEnabled = false;
		m_GaussianBlurEnabled = false;
		m_FXAAEnabled = false;
		m_SSAOEnabled = false;
		m_MeshesRendered = 0;
		m_TotalMeshes = 0;

		m_PostProcessSampler.Init(FilterType::Anisotropic, AddressType::Wrap);
		m_GBufferSampler.Init(FilterType::Bilinear, AddressType::Wrap);

		m_FullScreenWindow.Init(width, height);

		m_BoxBlurEffect.Init(width / 2, height / 2, 0.1f, 1000.f, width, height, BlurType::BoxBlur);
		m_GaussianBlurEffect.Init(width / 2, height / 2, 0.1f, 1000.f, width, height, BlurType::GaussianBlur);
		m_FXAA.Init((float) width, (float) height);
		m_SSAO.Init((float)width, (float)height);

		m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		m_MaterialSystem = std::make_unique<MaterialSystem>();
		m_TextureLibrary = std::make_unique<TextureLibrary>();

		TextureLibrary::Init();
		TextureLibrary::Add("Missing Texture", "src/Assets/Textures/ErrorTexture.png", TextureType::Tex2D);
		TextureLibrary::Add("Stone", "src/Assets/Textures/stone.png", TextureType::Tex2D);
		TextureLibrary::Add("Metal", "src/Assets/Textures/Metal.png", TextureType::Tex2D);
		TextureLibrary::Add("Noise", "src/Assets/Textures/Noise.png", TextureType::Tex2D);
		TextureLibrary::Add("Default Ambient", "src/Assets/Textures/DefaultAmbient.png", TextureType::Tex2D);
		TextureLibrary::Add("BoundingVolume", "src/Assets/Textures/BoundingVolumeTexture.png", TextureType::Tex2D);

		ShaderLibrary::Init();
		ShaderLibrary::Add("Test Shader", L"src/Shaders/CSO/TestVS.cso", L"src/Shaders/CSO/TestPS.cso");
		ShaderLibrary::Add("Texture Shader", L"src/Shaders/CSO/TextureVS.cso", L"src/Shaders/CSO/TexturePS.cso");
		ShaderLibrary::Add("Unlit Texture Shader", L"src/Shaders/CSO/UnlitTextureVS.cso", L"src/Shaders/CSO/UnlitTexturePS.cso");
		ShaderLibrary::Add("Color Shader", L"src/Shaders/CSO/DeferredVS.cso", L"src/Shaders/CSO/FlatColourPS.cso");
		ShaderLibrary::Add("Bounding Volume Shader", L"src/Shaders/CSO/BoundingVolumeVS.cso", L"src/Shaders/CSO/BoundingVolumePS.cso");

		ShaderLibrary::Add("Shadow Shader", L"src/Shaders/CSO/ShadowMapVS.cso", L"src/Shaders/CSO/ShadowMapPS.cso");
		ShaderLibrary::Add("Soft Shadow Shader", L"src/Shaders/CSO/SoftShadowVS.cso", L"src/Shaders/CSO/SoftShadowPS.cso");
		ShaderLibrary::Add("Phong Shader", L"src/Shaders/CSO/PhongDirVS.cso", L"src/Shaders/CSO/PhongDirPS.cso");
		ShaderLibrary::Add("Blinn-Phong Shader", L"src/Shaders/CSO/PhongDirVS.cso", L"src/Shaders/CSO/BlinnPhongPS.cso");
		ShaderLibrary::Add("PBR Shader", L"src/Shaders/CSO/PBR_VS.cso", L"src/Shaders/CSO/PBR_PS.cso");

		ShaderLibrary::Add("GBuffer Shader", L"src/Shaders/CSO/DeferredVS.cso", L"src/Shaders/CSO/DeferredPS.cso");
		ShaderLibrary::Add("Depth Shader", L"src/Shaders/CSO/DepthVS.cso", L"src/Shaders/CSO/DepthPS.cso");
		ShaderLibrary::Add("Gaussian Blur Shader", L"src/Shaders/CSO/PostProcessVS.cso", L"src/Shaders/CSO/GaussianBlurPS.cso");
		ShaderLibrary::Add("Box Blur Shader", L"src/Shaders/CSO/PostProcessVS.cso", L"src/Shaders/CSO/BoxBlurPS.cso");

		ShaderLibrary::Add("FXAA", L"src/Shaders/CSO/PostProcessVS.cso", L"src/Shaders/CSO/FXAA_PS.cso");
		ShaderLibrary::Add("SSAO", L"src/Shaders/CSO/SSAO_VS.cso", L"src/Shaders/CSO/SSAO_PS.cso");
		ShaderLibrary::Add("SSAO Blur", L"src/Shaders/CSO/SSAO_VS.cso", L"src/Shaders/CSO/SSAOBlur_PS.cso");
		ShaderLibrary::Add("Ambient Shader", L"src/Shaders/CSO/UnlitTextureVS.cso", L"src/Shaders/CSO/AmbientOccPS.cso");

		ShaderLibrary::AddCompute("Shadow Atlas", L"src/Shaders/CSO/Compute/ShadowAtlasCS.cso");

		MaterialSystem::Init();
		MaterialSystem::Add("Error Material", ShaderLibrary::Get("Test Shader"));
		MaterialSystem::Add("Flat Material", ShaderLibrary::Get("Unlit Texture Shader"));
		MaterialSystem::Add("Color Material", ShaderLibrary::Get("Unlit Texture Shader"));
		MaterialSystem::Add("Texture Material", ShaderLibrary::Get("Texture Shader"));
		MaterialSystem::Add("Depth Material", ShaderLibrary::Get("Depth Shader"));
		MaterialSystem::Add("GBuffer Material", ShaderLibrary::Get("GBuffer Shader"));
		MaterialSystem::Add("Shadow Map Material", ShaderLibrary::Get("Soft Shadow Shader"));
		MaterialSystem::Add("Phong Material", ShaderLibrary::Get("Phong Shader"));
		MaterialSystem::Add("Blinn-Phong Material", ShaderLibrary::Get("Blinn-Phong Shader"));
		MaterialSystem::Add("PBR Material", ShaderLibrary::Get("PBR Shader"));

		m_Ambient = TextureLibrary::GetTexture2D("Default Ambient")->GetTexture();
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
		
		renderable->ResetCulledCount();

		if(renderable->GetMaterialInstance()->IsIlluminated())
		{
			if (std::find(m_DepthRenderQueue.begin(), m_DepthRenderQueue.end(), renderable) == m_DepthRenderQueue.end())
				m_DepthRenderQueue.push_back(renderable);
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

	void Renderer::Render(Camera& camera, DirectX::XMMATRIX& lightViewProj, std::vector<PointLight>& pointLights)
	{
		if(m_CurrentDebugFrame == 0)
			RendererContext::GetDebug()->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);

		m_CurrentDebugFrame = (m_CurrentDebugFrame + 1) % m_UpdateDebug;

		m_MeshesRendered = 0;
		for (int i = 0; i < m_Renderables.size(); i++)
		{
			m_MeshesRendered += m_Renderables[i]->GetMeshCount();
		}

		m_TotalMeshes = m_MeshesRendered;

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		DirectX::XMMATRIX viewProj =
			DirectX::XMMatrixMultiply(view, proj);

		// Testing, using to construct shadow atlas
		//LightDepthPass(camera, pointLights);

		// Depth pre-pass, populating depth buffer
		DepthPrePass(camera, viewProj, m_DepthPass.get());

		// Calculating shadow map
		DepthPrePass(camera, lightViewProj, m_ShadowMap.get());

		// Soft shadows
		RenderShadows(camera);

		// Testing
		GBufferPass(camera);

		RendererContext::ClearRenderTarget(
			m_BackBufferColor[0],
			m_BackBufferColor[1],
			m_BackBufferColor[2],
			m_BackBufferColor[3]);

		if (m_PostProcessingEnabled)
		{
			RenderSceneToTexture(camera);
			PostProcessedScene(camera);

			return;
		}

		while(!m_OpaqueRenderQueue.empty())
		{
			Renderable* rPtr = m_OpaqueRenderQueue.front();

			rPtr->SetShadowMap(m_SoftShadow->GetSRV());
			rPtr->SetAmbientMap(m_Ambient);
			rPtr->Render(camera, viewProj, view, proj, false, m_BoundingVolumesEnabled);
			rPtr->GetMaterialInstance()->UnbindShaderResources();

			if(m_MeshesRendered > 0) m_MeshesRendered -= rPtr->GetCulledCount();

			m_OpaqueRenderQueue.pop();
		}

		// Enable alpha blending for transparent objects
		RendererContext::EnableAlphaBlending();

		while (!m_TransparentRenderQueue.empty())
		{
			Renderable* rPtr = m_TransparentRenderQueue.front();

			rPtr->SetShadowMap(m_SoftShadow->GetSRV());
			rPtr->SetAmbientMap(m_Ambient);
			rPtr->Render(camera, viewProj, view, proj, false, m_BoundingVolumesEnabled);
			rPtr->GetMaterialInstance()->UnbindShaderResources();

			if (m_MeshesRendered > 0) m_MeshesRendered -= rPtr->GetCulledCount();
			
			m_TransparentRenderQueue.pop();
		}

		RendererContext::DisableAlphaBlending();
	}

	void Renderer::DepthPrePass(Camera& camera, DirectX::XMMATRIX& lightViewProj, RenderToTexture* renderTex)
	{
		renderTex->SetRenderTarget();
		renderTex->ClearRenderTarget(0.f, 0.f, 0.f, 1.0f);

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		std::pair<VertexShader*, PixelShader*> depthShaders = ShaderLibrary::Get("Depth Shader");
		
		for(int i = 0; i < m_DepthRenderQueue.size(); i++)
		{
			Renderable* rPtr = m_DepthRenderQueue[i];

			depthShaders.first->Bind();
			depthShaders.second->Bind();

			rPtr->Render(camera, lightViewProj, view, proj, true, false, true);
		}

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Renderer::LightDepthPass(Camera& camera, std::vector<PointLight>& lights)
	{
		ComputeShader* atlasCS = ShaderLibrary::GetCompute("Shadow Atlas");

		std::unique_ptr<TextureArray> textureArr = std::make_unique<TextureArray>();
		textureArr->Init(SHADOW_ATLAS_BATCH_SIZE, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

		size_t numDispatches = (lights.size() + SHADOW_ATLAS_BATCH_SIZE - 1) / SHADOW_ATLAS_BATCH_SIZE;
		size_t numLightMaps = 0;
		ID3D11ShaderResourceView* srvs[SHADOW_ATLAS_BATCH_SIZE] = { nullptr };
		ID3D11Texture2D* textures[SHADOW_ATLAS_BATCH_SIZE] = { nullptr };;

		DirectX::XMMATRIX view, proj, viewProj;

		for(size_t dispatch = 0; dispatch < numDispatches; dispatch++)
		{
			ShadowAtlasType offsets;

			for(int i = 0; i < SHADOW_ATLAS_BATCH_SIZE; i++)
			{
				size_t lightIndex = dispatch * SHADOW_ATLAS_BATCH_SIZE + i;
				if (lightIndex > lights.size()) break;

				PointLight light = lights[lightIndex];
				light.GetView(view);
				light.GetProjection(proj);

				if (!camera.GetBoundingFrustum().Intersects(light.GetBoundingSphere())) continue;

				viewProj = DirectX::XMMatrixMultiply(view, proj);

				offsets.offSets[i] = DirectX::XMFLOAT2((float)((i % 8) * SHADOW_MAP_SIZE), (float)((i / 8) * SHADOW_MAP_SIZE));

				DepthPrePass(camera, viewProj, m_LightDepthMaps[lightIndex].get());

				srvs[i] = m_LightDepthMaps[lightIndex]->GetSRV();
				textures[i] = m_LightDepthMaps[lightIndex]->GetTextureResource();
				numLightMaps++;
			}
			
			textureArr->UpdateTextureArray(textures);

			m_ShadowAtlasBuffers[dispatch % m_ShadowAtlasBuffers.size()]->SetOffsets(offsets);
			m_ShadowAtlasBuffers[dispatch % m_ShadowAtlasBuffers.size()]->Update();
			m_ShadowAtlasBuffers[dispatch % m_ShadowAtlasBuffers.size()]->Bind(0);
			atlasCS->SetSRV(0, textureArr->GetTexture());
			atlasCS->SetUAV(0, m_ShadowAtlas->GetUAV());
			atlasCS->Bind();

			RendererContext::GetDeviceContext()->Dispatch(16, 16, 1);

			ID3D11ShaderResourceView* nullSRV = { nullptr };
			ID3D11UnorderedAccessView* nullUAV = { nullptr };
			RendererContext::GetDeviceContext()->CSSetShaderResources(0, 1, &nullSRV);
			RendererContext::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
		}
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
			
			gBufferShaders.first->Bind();
			gBufferShaders.second->Bind();

			rPtr->Render(camera, viewProj, view, proj, true);

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
		m_SceneTexture->ClearRenderTarget(
			m_BackBufferColor[0],
			m_BackBufferColor[1], 
			m_BackBufferColor[2], 
			m_BackBufferColor[3]);

		for(int i = 0; i < m_Renderables.size(); i++)
		{
			Renderable* rPtr = m_Renderables[i];

			rPtr->SetShadowMap(m_SoftShadow->GetSRV());
			rPtr->SetAmbientMap(m_Ambient);
			rPtr->Render(camera, viewProj, view, proj, false, m_BoundingVolumesEnabled);
			rPtr->GetMaterialInstance()->UnbindShaderResources();
		}

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Renderer::PostProcessedScene(Camera& camera)
	{
		RendererContext::DisableZBuffer();

		ID3D11ShaderResourceView* ambient;

		if (m_BoxBlurEnabled) 
			m_BoxBlurEffect.BlurScene(camera, m_SceneTexture.get());
		
		if (m_GaussianBlurEnabled) 
			m_GaussianBlurEffect.BlurScene(camera, m_SceneTexture.get());
		
		if (m_FXAAEnabled) 
			m_FXAA.Execute(camera, m_SceneTexture.get());

		if (m_SSAOEnabled)
		{
			m_SSAO.Execute(camera, m_GBuffer->GetSRV(Buffer::ViewPosition),
				m_GBuffer->GetSRV(Buffer::Normal), TextureLibrary::GetTexture2D("Noise")->GetTexture());
			ambient = m_SSAO.GetAmbient();
		}
		else
		{
			ambient = TextureLibrary::GetTexture2D("Default Ambient")->GetTexture();
		}


		std::pair<VertexShader*, PixelShader*> textureShader = ShaderLibrary::Get("Ambient Shader");

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMMATRIX viewProj;

		camera.GetDefaultView(view);
		m_SceneTexture->GetOrthoMatrix(proj);

		viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_FullScreenWindow.Render(viewProj, view, proj);
		textureShader.second->SetTexture(0, m_SceneTexture->GetSRV());
		textureShader.second->SetTexture(1, ambient);
		m_PostProcessSampler.Bind(0);
		textureShader.first->Bind();
		textureShader.second->Bind();
		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		RendererContext::EnableZBuffer();
	}

	void Renderer::RenderShadows(Camera& camera)
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetViewMatrix(view);
		camera.GetProjectionMatrix(proj);

		DirectX::XMMATRIX viewProj =
			DirectX::XMMatrixMultiply(view, proj);

		m_SoftShadow->SetRenderTarget();
		m_SoftShadow->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		std::pair<VertexShader*, PixelShader*> shadowShaders = ShaderLibrary::Get("Shadow Shader");

		RendererContext::EnableAlphaBlending();

		for (int i = 0; i < m_Renderables.size(); i++)
		{
			Renderable* rPtr = m_Renderables[i];

			shadowShaders.second->SetTexture(TextureSlot::ShadowMapTexture, m_ShadowMap->GetSRV());
			m_PostProcessSampler.Bind(SamplerSlot::ClampSampler);
			shadowShaders.first->Bind();
			shadowShaders.second->Bind();
			rPtr->GetMaterialInstance()->GetLightPropsBuffer()->Bind(PSBufferSlot::LightProperties);
			rPtr->Render(camera, viewProj, view, proj, true);
			ID3D11ShaderResourceView* nullSRV = { nullptr };
			RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		}

		RendererContext::DisableAlphaBlending();

		m_GaussianBlurEffect.BlurScene(camera, m_SoftShadow.get());

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}
}