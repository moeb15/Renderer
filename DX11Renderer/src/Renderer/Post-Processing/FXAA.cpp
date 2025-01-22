#include "Renderer/Post-Processing/FXAA.h"

namespace Yassin
{
	void FXAA::Init(float sWidth, float sHeight)
	{
		m_ScreenBuffer = std::make_unique<ScreenBuffer>();
		m_ScreenBuffer->SetWidth(sWidth);
		m_ScreenBuffer->SetHeight(sHeight);

		m_RenderWindow.Init((unsigned int)sWidth, (unsigned int)sHeight);
		m_Sampler.Init(FilterType::Bilinear, AddressType::Clamp);

		m_RenderTex.Init(sWidth, sHeight, 0.1f, 1000.f, RenderTargetType::DepthStencil);
	}

	void FXAA::Execute(Camera& camera, RenderToTexture* renderTex)
	{
		std::pair<VertexShader*, PixelShader*> fxaaShaders = ShaderLibrary::Get("FXAA");
		std::pair<VertexShader*, PixelShader*> textureShader = ShaderLibrary::Get("Unlit Texture Shader");

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetDefaultView(view);
		m_RenderTex.GetOrthoMatrix(proj);

		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_RenderTex.SetRenderTarget();
		m_RenderTex.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		m_ScreenBuffer->Update();
		m_RenderWindow.Render(viewProj);
		fxaaShaders.second->SetTexture(TextureSlot::BaseTexture, renderTex->GetSRV());
		m_Sampler.Bind(SamplerSlot::WrapSampler);
		m_ScreenBuffer->Bind(0);
		fxaaShaders.first->Bind();
		fxaaShaders.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		renderTex->SetRenderTarget();
		renderTex->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		renderTex->GetOrthoMatrix(proj);

		viewProj = DirectX::XMMatrixMultiply(view, proj);
		m_RenderWindow.Render(viewProj);

		m_Sampler.Bind(0);
		textureShader.second->SetTexture(0, m_RenderTex.GetSRV());
		textureShader.first->Bind();
		textureShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}
}
