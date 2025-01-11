#include "Renderer/Post-Processing/Blur.h"
#include "Renderer/Resources/ShaderLibrary.h"
#include "Renderer/Resources/MaterialInstance.h"

namespace Yassin
{
	void Blur::Init(unsigned int dsWidth, unsigned int dsHeight, 
		float sNear, float sFar, unsigned int rWidth, 
		unsigned int rHeight, BlurType type)
	{
		switch(type)
		{
			case BlurType::GaussianBlur: 
				m_BlurShaderName = "Gaussian Blur Shader"; 
				break;

			case BlurType::BoxBlur:
				m_BlurShaderName = "Box Blur Shader";
				break;

			default:
				m_BlurShaderName = "Gaussian Blur Shader";
				break;
		}

		m_DSWidth = dsWidth;
		m_DSHeight = dsHeight;

		m_DownSampleTex1.Init(m_DSWidth, m_DSHeight, sNear, sFar, RenderTargetType::DepthStencil);
		m_DownSampleTex2.Init(m_DSWidth, m_DSHeight, sNear, sFar, RenderTargetType::DepthStencil);

		m_DownSampleWindow.Init(dsWidth, dsHeight);
		m_UpSampleWindow.Init(rWidth, rHeight);

		m_Sampler = Sampler();
		m_Sampler.Init(FilterType::Anisotropic, AddressType::Clamp);
		m_ScreenBuffer = std::make_unique<ScreenBuffer>();
	}

	void Blur::BlurScene(Camera& camera, RenderToTexture* renderTex)
	{
		DirectX::XMMATRIX view, ortho, viewProj;

		camera.GetDefaultView(view);
		
		std::pair<VertexShader*, PixelShader*> blurShader = ShaderLibrary::Get(m_BlurShaderName);
		std::pair<VertexShader*, PixelShader*> textureShader = ShaderLibrary::Get("Unlit Texture Shader");

		// rendering scene to downsampled texture
		m_DownSampleTex1.SetRenderTarget();
		m_DownSampleTex1.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		m_DownSampleTex1.GetOrthoMatrix(ortho);

		viewProj = DirectX::XMMatrixMultiply(view, ortho);
		m_DownSampleWindow.Render(viewProj);

		m_Sampler.Bind(0);
		textureShader.second->SetTexture(0, renderTex->GetSRV());
		textureShader.first->Bind();
		textureShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		ID3D11ShaderResourceView* nullSRV = { nullptr };
		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		// apply horizontal blur

		m_ScreenBuffer->SetHorizontalBlur();
		m_ScreenBuffer->Update();

		m_DownSampleTex2.SetRenderTarget();
		m_DownSampleTex2.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		m_DownSampleTex2.GetOrthoMatrix(ortho);

		viewProj = DirectX::XMMatrixMultiply(view, ortho);

		m_DownSampleWindow.Render(viewProj);

		blurShader.second->SetTexture(TextureSlot::BaseTexture, m_DownSampleTex1.GetSRV());
		m_Sampler.Bind(0);
		m_ScreenBuffer->Bind(0);
		blurShader.first->Bind();
		blurShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		// apply vertical blur

		m_ScreenBuffer->SetVerticalBlur();
		m_ScreenBuffer->Update();

		m_DownSampleTex1.SetRenderTarget();
		m_DownSampleTex1.ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		m_DownSampleTex2.GetOrthoMatrix(ortho);

		viewProj = DirectX::XMMatrixMultiply(view, ortho);

		blurShader.second->SetTexture(TextureSlot::BaseTexture, m_DownSampleTex2.GetSRV());
		m_Sampler.Bind(0);
		m_ScreenBuffer->Bind(0);
		blurShader.first->Bind();
		blurShader.second->Bind();

		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		renderTex->SetRenderTarget();
		renderTex->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);
		renderTex->GetOrthoMatrix(ortho);

		viewProj = DirectX::XMMatrixMultiply(view, ortho);
		m_UpSampleWindow.Render(viewProj);

		m_Sampler.Bind(0);
		textureShader.second->SetTexture(0, m_DownSampleTex1.GetSRV());
		textureShader.first->Bind();
		textureShader.second->Bind();
		
		RendererContext::GetDeviceContext()->DrawIndexed(6, 0, 0);

		RendererContext::GetDeviceContext()->PSSetShaderResources(0, 1, &nullSRV);

		RendererContext::SetBackBufferRenderTarget();
		RendererContext::ResetViewport();
	}

	void Blur::ChangeBlurType(BlurType type)
	{
		switch (type)
		{
		case BlurType::GaussianBlur:
			m_BlurShaderName = "Gaussian Blur Shader";
			break;

		case BlurType::BoxBlur:
			m_BlurShaderName = "Box Blur Shader";
			break;

		default:
			m_BlurShaderName = "Gaussian Blur Shader";
			break;
		}
	}
}