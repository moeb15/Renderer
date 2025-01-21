#pragma once
// TODO clean up class, getting very bloated, 
// should not handle much outside of submission of renderables and rendering

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include "Renderer/RendererContext.h"
#include "Renderer/Renderable.h"
#include "Renderer/Resources/ShaderLibrary.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Resources/MaterialSystem.h"
#include "Renderer/Resources/TextureLibrary.h"
#include "Renderer/RenderToTexture.h"
#include "Renderer/GBuffer.h"
#include "Renderer/RenderToUAV.h"
#include "Renderer/Post-Processing/Blur.h"
#include "Renderer/Post-Processing/FXAA.h"
#include "Renderable/Light/PointLight.h"
#include "Renderer/Primitives/TextureArray.h"

#include <queue>
#include <deque>

namespace Yassin
{
	class Renderer
	{
	public:
		void Init(int width, int height, HWND hWnd, bool fullscreen = true);
		void BeginScene(float r, float g, float b, float a);
		void EndScene();
		void Submit(Renderable* renderable);
		void Render(Camera& camera, DirectX::XMMATRIX& lightViewProj, std::vector<PointLight>& pointLights);

		inline void TogglePostProcessing() { m_PostProcessingEnabled = !m_PostProcessingEnabled; }
		inline void ToggleDeferredRendering() { m_DeferredRenderingEnabled = !m_DeferredRenderingEnabled; }
		inline void ToggleBoundingVolumes() { m_BoundingVolumesEnabled = !m_BoundingVolumesEnabled; }

		inline bool& PostProcessingEnabled() { return m_PostProcessingEnabled; }
		inline bool& GaussianBlurEnabled() { return m_GaussianBlurEnabled; }
		inline bool& BoxBlurEnabled() { return m_BoxBlurEnabled; }
		inline bool& FXAAEnabled() { return m_FXAAEnabled; }
		inline bool& SSAOEnabled() { return m_SSAOEnabled; }
		inline size_t GetMeshesRendered() const { return m_MeshesRendered; }
		inline size_t GetTotalMeshes() const { return m_TotalMeshes; }

	private:
		void DepthPrePass(Camera& camera, DirectX::XMMATRIX& lightViewProj, RenderToTexture* renderTex);
		void LightDepthPass(Camera& camera, std::vector<PointLight>& lights);
		void GBufferPass(Camera& camera);
		void RenderSceneToTexture(Camera& camera);
		void PostProcessedScene(Camera& camera);
		void RenderShadows(Camera& camera);

	private:
		std::unique_ptr<TextureLibrary> m_TextureLibrary;
		std::unique_ptr<MaterialSystem> m_MaterialSystem;
		std::unique_ptr<ShaderLibrary> m_ShaderLibrary;
		std::unique_ptr<RendererContext> m_Context;
		std::unique_ptr<RenderToTexture> m_DepthPass;
		std::unique_ptr<RenderToTexture> m_ShadowMap;
		std::unique_ptr<RenderToTexture> m_SoftShadow;
		std::unique_ptr<RenderToTexture> m_SceneTexture;
		std::unique_ptr<GBuffer> m_GBuffer;
		std::unique_ptr<RenderToUAV> m_ShadowAtlas;
		std::unique_ptr<TextureArray> m_ShadowMapArray;
		std::vector<std::unique_ptr<ShadowAtlasBuffer>> m_ShadowAtlasBuffers;
		std::vector<std::unique_ptr<RenderToTexture>> m_LightDepthMaps;

		Blur m_GaussianBlurEffect;
		Blur m_BoxBlurEffect;
		FXAA m_FXAA;
		OrthoWindow m_FullScreenWindow;
		Sampler m_PostProcessSampler;
		Sampler m_GBufferSampler;

		std::queue<Renderable*> m_OpaqueRenderQueue;
		std::queue<Renderable*> m_TransparentRenderQueue;
		std::vector<Renderable*> m_DepthRenderQueue;
		std::queue<Renderable*> m_GBufferQueue;
		std::deque<Renderable*> m_Renderables;

		float m_BackBufferColor[4];
		size_t m_MeshesRendered;
		size_t m_TotalMeshes;
		bool m_PostProcessingEnabled;
		bool m_GaussianBlurEnabled;
		bool m_BoxBlurEnabled;
		bool m_FXAAEnabled;
		bool m_SSAOEnabled;
		bool m_DeferredRenderingEnabled;
		bool m_BoundingVolumesEnabled;

		unsigned int m_UpdateDebug;
		unsigned int m_CurrentDebugFrame;
	};
}