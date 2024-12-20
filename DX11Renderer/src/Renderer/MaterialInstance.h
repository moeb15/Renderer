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

		void SetTexture(unsigned int slot, const std::string& texture);
		void SetSampler(unsigned int slot, FilterType fType, AddressType aType);

		inline VertexShader* GetVertexShader() { return m_VertexShader; }
		inline PixelShader* GetPixelShader() { return m_PixelShader; }

		void BindMaterial();

	private:
		std::unordered_map<unsigned int, Texture2D*> m_Textures;
		std::unordered_map<unsigned int, std::unique_ptr<Sampler>> m_Samplers;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
	};
}

