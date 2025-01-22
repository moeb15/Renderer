#pragma once
#include "Renderer/RenderToTexture.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/Renderable.h"
#include "Renderable/Utils/OrthoWindow.h"
#include <memory>

namespace Yassin
{
	class SSAO
	{
	public:
		void Init(float sWidth, float sHeight);

		void Execute(Camera& camera, ID3D11ShaderResourceView* pos,
			ID3D11ShaderResourceView* norm, ID3D11ShaderResourceView* random);

		inline ID3D11ShaderResourceView* GetAmbient() { return m_BlurSSAOTex.GetSRV(); }

	private:
		std::unique_ptr<ScreenBuffer> m_ScreenBuffer;
		std::unique_ptr<SSAOPropertiesBuffer> m_SSAOProps;
		RenderToTexture m_SSAOTex;
		RenderToTexture m_BlurSSAOTex;
		OrthoWindow m_RenderWindow;
		Sampler m_Wrap;
		Sampler m_Clamp;
		Sampler m_Point;
	};
}