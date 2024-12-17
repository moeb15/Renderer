#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Material.h"

namespace Yassin
{
	class MaterialInstance
	{
	public:
		MaterialInstance() = delete;
		MaterialInstance(const Material&) = delete;
		MaterialInstance& operator=(const MaterialInstance&) = delete;

		MaterialInstance(Material* material);

		void SetTexture(unsigned int slot, const char* textureFile);
		void SetSampler(unsigned int slot, FilterType fType, AddressType aType);

		void BindMaterial();

	private:
		std::unordered_map<unsigned int, std::shared_ptr<Texture2D>> m_Textures;
		std::unordered_map<unsigned int, std::shared_ptr<Sampler>> m_Samplers;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
	};
}

