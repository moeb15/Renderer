#pragma once
#include "Renderer/RenderToTexture.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Renderable.h"
#include "Renderable/Utils/OrthoWindow.h"
#include <memory>

namespace Yassin
{
	class FXAA
	{
	public:
		void Init(float sWidth, float sHeight);
		
		void Execute(Camera& camera, RenderToTexture* renderTex);

	private:
		std::unique_ptr<ScreenBuffer> m_ScreenBuffer;
		RenderToTexture m_RenderTex;
		OrthoWindow m_RenderWindow;
		Sampler m_Sampler;
	};
}