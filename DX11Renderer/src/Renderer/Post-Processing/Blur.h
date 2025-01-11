#pragma once
#include "Renderer/RenderToTexture.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderable.h"
#include "Renderable/Utils/OrthoWindow.h"
#include <memory>
#include <deque>
#include <string>

namespace Yassin
{
	class Blur
	{
	public:
		void Init(unsigned int dsWidth, unsigned int dsHeight,
			float sNear, float sFar, unsigned int rWidth,
			unsigned int rHeight, BlurType type);

		void BlurScene(Camera& camera, RenderToTexture* renderTex);
		void ChangeBlurType(BlurType type);

	private:
		RenderToTexture m_DownSampleTex1;
		RenderToTexture m_DownSampleTex2;
		std::unique_ptr<ScreenBuffer> m_ScreenBuffer;
		Sampler m_Sampler;
		OrthoWindow m_DownSampleWindow;
		OrthoWindow m_UpSampleWindow;
		unsigned int m_DSWidth;
		unsigned int m_DSHeight;
		std::string m_BlurShaderName;
	};
}
