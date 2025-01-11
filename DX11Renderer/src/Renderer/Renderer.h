#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include "Renderer/RendererContext.h"
#include "Renderer/Renderable.h"
#include "Renderer/Resources/ShaderLibrary.h"
#include "Renderer/Camera.h"
#include "Renderer/Resources/MaterialSystem.h"
#include "Renderer/Resources/TextureLibrary.h"
#include "Renderer/RenderToTexture.h"
#include "Renderer/Post-Processing/Blur.h"
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
		void Render(Camera& camera, DirectX::XMMATRIX& lightViewProj);

		inline void TogglePostProcessing() { m_PostProcessingEnabled = !m_PostProcessingEnabled; }

	private:
		void DepthPrePass(DirectX::XMMATRIX& lightViewProj);
		void RenderSceneToTexture(Camera& camera);

	private:
		std::unique_ptr<TextureLibrary> m_TextureLibrary;
		std::unique_ptr<MaterialSystem> m_MaterialSystem;
		std::unique_ptr<ShaderLibrary> m_ShaderLibrary;
		std::unique_ptr<RendererContext> m_Context;
		std::unique_ptr<RenderToTexture> m_DepthPass;
		std::unique_ptr<RenderToTexture> m_SceneTexture;
		Blur m_BlurEffect;
		OrthoWindow m_FullScreenWindow;
		Sampler m_PostProcessSampler;

		std::queue<Renderable*> m_OpaqueRenderQueue;
		std::queue<Renderable*> m_TransparentRenderQueue;
		std::queue<Renderable*> m_DepthRenderQueue;
		std::deque<Renderable*> m_Renderables;

		float m_BackBufferColor[4];
		bool m_PostProcessingEnabled;
	};
}