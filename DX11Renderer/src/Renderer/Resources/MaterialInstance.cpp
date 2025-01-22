#include "Renderer/Resources/MaterialInstance.h"
#include "Renderer/Resources/TextureLibrary.h"
#include "MaterialInstance.h"


namespace Yassin
{
	MaterialInstance::MaterialInstance(Material* material)
	{
		if(material->IsIlluminated())
		{
			m_LightPosBuffer = std::make_unique<LightPosBuffer>();
			m_LightPropsBuffer = std::make_unique<LightPropsBuffer>();
			m_Illuminated = true;
		}
		else 
		{
			m_LightPosBuffer = nullptr;
			m_LightPropsBuffer = nullptr;
			m_Illuminated = false;
		}

		if(material->IsTransparent())
		{
			m_TransparencyBuffer = std::make_unique<TransparencyBuffer>();
			m_TransparencyBuffer->UpdateBuffer({});
			m_Transparent = true;
		}
		else
		{
			m_TransparencyBuffer = nullptr;
			m_Transparent = false;
		}

		m_LightDirectionBuffer = std::make_unique<LightDirectionBuffer>();
		m_CameraBuffer = std::make_unique<CameraBuffer>();

		for(const TextureMetaData& tData : material->GetTextures())
		{
			m_Textures.emplace(tData.slot, nullptr);
		}

		for(const SamplerMetaData& sData : material->GetSamplers())
		{
			std::unique_ptr<Sampler> sampler = std::make_unique<Sampler>();
			m_Samplers.emplace(std::pair<unsigned int, std::unique_ptr<Sampler>>(sData.slot, std::move(sampler)));
		}

		m_VertexShader = material->GetVertexShader();
		m_PixelShader = material->GetPixelShader();
		m_Name = material->GetName();
	}

	void MaterialInstance::SetTexture(unsigned int slot, const std::string& texture)
	{
		if (m_Textures.find(slot) == m_Textures.end())
		{
			if(slot < TextureSlot::TEXTURE_COUNT)
				m_Textures.emplace(slot, nullptr);
			else
				return;
		}
		m_Textures.find(slot)->second = TextureLibrary::GetTexture2D(texture);
	}

	void MaterialInstance::SetSampler(unsigned int slot, FilterType fType, AddressType aType)
	{
		if (m_Samplers.find(slot) == m_Samplers.end()) return;
		
		m_Samplers.find(slot)->second->Init(fType, aType);
	}

	void MaterialInstance::SetShadowMap(ID3D11ShaderResourceView* srv)
	{
		RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::ShadowMapTexture, 1, &srv);
	}

	void MaterialInstance::SetAmbientMap(ID3D11ShaderResourceView* srv)
	{
		RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::AmbientMap, 1, &srv);
	}

	ID3D11ShaderResourceView* MaterialInstance::GetTexture(unsigned int slot)
	{
		if (m_Textures.find(slot) == m_Textures.end()) return nullptr;
		return m_Textures.find(slot)->second->GetTexture();
	}

	void MaterialInstance::UpdateLightBuffers(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps)
	{
		if(m_Illuminated)
		{
			m_LightPosBuffer->UpdateBuffer(lPos);
		}

		if(m_Illuminated)
		{
			m_LightPropsBuffer->UpdateBuffer(lProps);
		}
	}

	void MaterialInstance::UpdateTransparencyBuffer(const BlendBuffer& tBuffer)
	{
		if(m_Transparent) m_TransparencyBuffer->UpdateBuffer(tBuffer);
	}

	void MaterialInstance::UpdateCameraBuffer(const CameraPositionType& cPos)
	{
		m_CameraBuffer->UpdateBuffer(cPos);
	}

	void MaterialInstance::UpdateLightDirection(const LightDirectionType& lDir)
	{
		m_LightDirectionBuffer->UpdateBuffer(lDir);
	}

	void MaterialInstance::BindMaterial()
	{
		if(m_Illuminated) m_LightPosBuffer->Bind(VSBufferSlot::LightPosition);
		if(m_Illuminated) m_LightPropsBuffer->Bind(PSBufferSlot::LightProperties);
		if(m_Transparent) m_TransparencyBuffer->Bind(PSBufferSlot::Transparency);

		m_CameraBuffer->Bind(VSBufferSlot::CameraPosition);
		m_LightDirectionBuffer->Bind(PSBufferSlot::LightDirection);

		for(const auto& kvPair : m_Textures)
		{
			if(m_Illuminated)
			{
				if (kvPair.first == TextureSlot::ShadowMapTexture) continue;
			}
			if (kvPair.first == TextureSlot::AmbientMap) continue;
			if (kvPair.second != nullptr) kvPair.second->Bind(kvPair.first);
		}

		for(const auto& kvPair : m_Samplers) kvPair.second->Bind(kvPair.first);

		m_VertexShader->Bind();
		m_PixelShader->Bind();
	}

	void MaterialInstance::BindShaderResources()
	{
		for (const auto& kvPair : m_Textures)
		{
			if (m_Illuminated)
			{
				if (kvPair.first == TextureSlot::ShadowMapTexture) continue;
			}
			if (kvPair.first == TextureSlot::AmbientMap) continue;
			if (kvPair.second != nullptr) kvPair.second->Bind(kvPair.first);
		}
		for (const auto& kvPair : m_Samplers) kvPair.second->Bind(kvPair.first);
	}

	void MaterialInstance::UnbindShaderResources()
	{
		ID3D11ShaderResourceView* nullSRV = { nullptr };
		for(unsigned int i = 0; i < TextureSlot::TEXTURE_COUNT; i++)
		{
			RendererContext::GetDeviceContext()->PSSetShaderResources(i, 1, &nullSRV);
		}
	}
}