#include "Renderer/MaterialInstance.h"
#include "MaterialInstance.h"
#include "Renderer/TextureLibrary.h"


namespace Yassin
{
	MaterialInstance::MaterialInstance(Material* material)
	{
		// TOOD: iterate over material constant buffers to create constant buffers for material instance
		// Update : See MaterialSystem.cpp as to why CBuffers were removed
		/*for (const CBufferMetaData& cData : material->GetCBuffers())
		{
			continue;
		}*/

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
	}

	void MaterialInstance::SetTexture(unsigned int slot, const std::string& texture)
	{
		if (m_Textures.find(slot) == m_Textures.end()) return;
		// TODO : switch based off TextureType
		m_Textures.find(slot)->second = TextureLibrary::GetTexture2D(texture);
	}

	void MaterialInstance::SetSampler(unsigned int slot, FilterType fType, AddressType aType)
	{
		if (m_Samplers.find(slot) == m_Samplers.end()) return;
		
		m_Samplers.find(slot)->second->Init(fType, aType);
	}

	void MaterialInstance::SetShadowMap(ID3D11ShaderResourceView* srv)
	{
		RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::DepthMapTexture, 1, &srv);
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

	void MaterialInstance::BindMaterial()
	{
		if(m_Illuminated) m_LightPosBuffer->Bind(VSBufferSlot::LightPosition);
		if(m_Illuminated) m_LightPropsBuffer->Bind(PSBufferSlot::LightProperties);
		if(m_Transparent) m_TransparencyBuffer->Bind(PSBufferSlot::Transparency);

		for(const auto& kvPair : m_Textures)
		{
			if(m_Illuminated)
			{
				if (kvPair.first == TextureSlot::DepthMapTexture) continue;
			}
			kvPair.second->Bind(kvPair.first);
		}

		for(const auto& kvPair : m_Samplers) kvPair.second->Bind(kvPair.first);

		m_VertexShader->Bind();
		m_PixelShader->Bind();
	}

	void MaterialInstance::UnbindShaderResources()
	{
		ID3D11ShaderResourceView* nullSRV = { nullptr };
		for(const auto& kvPair : m_Textures)
		{
			RendererContext::GetDeviceContext()->PSSetShaderResources(kvPair.first, 1, &nullSRV);
		}
	}
}