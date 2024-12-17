#include "Renderer/MaterialInstance.h"
#include "MaterialInstance.h"


namespace Yassin
{
	MaterialInstance::MaterialInstance(Material* material)
	{
		// TOOD: iterate over material constant buffers to create constant buffers for material instance
		for(const CBufferMetaData& cData : material->GetCBuffers())
		{
			continue;
		}

		for(const TextureMetaData& tData : material->GetTextures())
		{
			std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
			m_Textures.emplace(std::pair<unsigned int, std::shared_ptr<Texture2D>>(tData.slot, std::move(texture)));
		}

		for(const SamplerMetaData& sData : material->GetSamplers())
		{
			std::shared_ptr<Sampler> sampler = std::make_shared<Sampler>();
			m_Samplers.emplace(std::pair<unsigned int, std::shared_ptr<Sampler>>(sData.slot, std::move(sampler)));
		}

		m_VertexShader = material->GetVertexShader();
		m_PixelShader = material->GetPixelShader();
	}

	void MaterialInstance::SetTexture(unsigned int slot, const char* textureFile)
	{
		if (m_Textures.find(slot) == m_Textures.end()) return;

		m_Textures.find(slot)->second->Init(textureFile);
	}

	void MaterialInstance::SetSampler(unsigned int slot, FilterType fType, AddressType aType)
	{
		if (m_Samplers.find(slot) == m_Samplers.end()) return;

		m_Samplers.find(slot)->second->Init(fType, aType);
	}

	void MaterialInstance::BindMaterial()
	{
		for(const auto& kvPair : m_Textures)
		{
			kvPair.second->Bind(kvPair.first);
		}

		for(const auto& kvPair : m_Samplers)
		{
			kvPair.second->Bind(kvPair.first);
		}

		m_VertexShader->Bind();
		m_PixelShader->Bind();
	}
}