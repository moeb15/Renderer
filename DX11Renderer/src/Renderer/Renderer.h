#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include "Renderer/RendererContext.h"
#include "Renderer/Renderable.h"
#include "Renderer/ShaderLibrary.h"
#include "Renderer/Camera.h"
#include "Renderer/MaterialSystem.h"
#include "Renderer/TextureLibrary.h"
#include <queue>

namespace Yassin
{
	class Renderer
	{
	public:
		void Init(int width, int height, HWND hWnd, bool fullscreen = true);
		void BeginScene(float r, float g, float b, float a);
		void SetBackBufferRenderTarget();
		void ResizeBuffer(unsigned int width, unsigned int height);
		void EndScene();
		void Submit(Renderable* renderable);
		void Render(Camera& camera);

	private:
		std::unique_ptr<TextureLibrary> m_TextureLibrary;
		std::unique_ptr<MaterialSystem> m_MaterialSystem;
		std::unique_ptr<ShaderLibrary> m_ShaderLibrary;
		std::unique_ptr<RendererContext> m_Context;
		std::queue<Renderable*> m_OpaqueRenderQueue;
		std::queue<Renderable*> m_TransparentRenderQueue;
	};
}