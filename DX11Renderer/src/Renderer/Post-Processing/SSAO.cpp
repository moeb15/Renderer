#include "Renderer/Post-Processing/SSAO.h"

namespace Yassin 
{
	void SSAO::Init(float sWidth, float sHeight)
	{
		m_SSAOProps = std::make_unique<SSAOPropertiesBuffer>();

		m_ScreenBuffer = std::make_unique<ScreenBuffer>();
		m_ScreenBuffer->SetWidth(sWidth);
		m_ScreenBuffer->SetHeight(sHeight);

		m_RenderWindow.Init((unsigned int)sWidth, (unsigned int)sHeight);
		m_Wrap.Init(FilterType::Bilinear, AddressType::Wrap);
		m_Clamp.Init(FilterType::Bilinear, AddressType::Clamp);
		m_Point.Init(FilterType::Point, AddressType::Clamp);

		m_SSAOTex.Init((unsigned int)sWidth, (unsigned int)sHeight, 0.1f, 1000.f, RenderTargetType::DepthStencil);
		m_BlurSSAOTex.Init((unsigned int)sWidth, (unsigned int)sHeight, 0.1f, 1000.f, RenderTargetType::DepthStencil);

	}

	void SSAO::Execute(Camera& camera, ID3D11ShaderResourceView* pos,
		ID3D11ShaderResourceView* norm, ID3D11ShaderResourceView* random)
	{
		std::pair<VertexShader*, PixelShader*> ssaoShader = ShaderLibrary::Get("SSAO");
		std::pair<VertexShader*, PixelShader*> ssaoBlurShader = ShaderLibrary::Get("SSAO Blur");

		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;

		camera.GetDefaultView(view);
		m_SSAOTex.GetOrthoMatrix(proj);

		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_SSAOTex.SetRenderTarget();
		m_SSAOTex.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

		m_SSAOProps->Update();
		m_RenderWindow.Render(viewProj, view, proj);

		ssaoShader.second->SetTexture(TextureSlot::BaseTexture, pos);
		ssaoShader.second->SetTexture(TextureSlot::NormalTexture, norm);
		ssaoShader.second->SetTexture(TextureSlot::SpecularTexture, random);
		m_Wrap.Bind(SamplerSlot::WrapSampler);
		m_Clamp.Bind(SamplerSlot::ClampSampler);
		m_SSAOProps->Bind(0);
		ssaoShader.first->Bind();
		ssaoShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		RendererContext::GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);
		RendererContext::GetDeviceContext()->PSSetShaderResources(2, 1, &nullSRV);

		m_BlurSSAOTex.SetRenderTarget();
		m_BlurSSAOTex.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		m_BlurSSAOTex.GetOrthoMatrix(proj);

		m_ScreenBuffer->SetHorizontalBlur();
		m_ScreenBuffer->Update();

		viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_RenderWindow.Render(viewProj, view, proj);
		m_ScreenBuffer->Bind(0);
		m_Point.Bind(SamplerSlot::PointSampler);
		ssaoBlurShader.second->SetTexture(TextureSlot::BaseTexture, m_SSAOTex.GetSRV());
		ssaoBlurShader.second->SetTexture(TextureSlot::NormalTexture, norm);
		ssaoBlurShader.first->Bind();
		ssaoBlurShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		RendererContext::GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);

		m_SSAOTex.SetRenderTarget();
		m_SSAOTex.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		m_SSAOTex.GetOrthoMatrix(proj);

		m_ScreenBuffer->SetVerticalBlur();
		m_ScreenBuffer->Update();

		viewProj = DirectX::XMMatrixMultiply(view, proj);

		m_RenderWindow.Render(viewProj, view, proj);
		m_ScreenBuffer->Bind(0);
		m_Point.Bind(SamplerSlot::PointSampler);
		ssaoBlurShader.second->SetTexture(TextureSlot::BaseTexture, m_BlurSSAOTex.GetSRV());
		ssaoBlurShader.second->SetTexture(TextureSlot::NormalTexture, norm);
		ssaoBlurShader.first->Bind();
		ssaoBlurShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);
		RendererContext::GetDeviceContext()->PSSetShaderResources(1, 1, &nullSRV);

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}
}
