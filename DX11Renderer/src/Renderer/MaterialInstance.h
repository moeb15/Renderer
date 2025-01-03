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
		void SetShadowMap(unsigned int slot, ID3D11ShaderResourceView* srv);

		inline bool IsIlluminated() const { return m_Illuminated; }

		void UpdateLightBuffers(const LightPositionBuffer& lPos = {}, const LightPropertiesBuffer& lProps = {});
		void UpdateTransparencyBuffer(const BlendBuffer& tBuffer = {});

		inline VertexShader* GetVertexShader() { return m_VertexShader; }
		inline PixelShader* GetPixelShader() { return m_PixelShader; }

		void BindMaterial();

	private:
		std::unordered_map<unsigned int, Texture2D*> m_Textures;
		std::unordered_map<unsigned int, std::unique_ptr<Sampler>> m_Samplers;
		std::unique_ptr<LightPosBuffer> m_LightPosBuffer;
		std::unique_ptr<LightPropsBuffer> m_LightPropsBuffer;
		std::unique_ptr<TransparencyBuffer> m_TransparencyBuffer;
		bool m_Illuminated;
		bool m_Transparent;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
	};
}

