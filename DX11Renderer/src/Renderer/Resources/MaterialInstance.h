#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Resources/Material.h"

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
		void SetShadowMap(ID3D11ShaderResourceView* srv);

		ID3D11ShaderResourceView* GetTexture(unsigned int slot);

		inline bool IsIlluminated() const { return m_Illuminated; }

		void UpdateLightBuffers(const LightPositionBuffer& lPos = {}, const LightPropertiesBuffer& lProps = {});
		void UpdateTransparencyBuffer(const BlendBuffer& tBuffer = {});
		void UpdateCameraBuffer(const CameraPositionType& cPos = {});
		void UpdateLightDirection(const LightDirectionType& lDir = {});

		inline LightPropsBuffer* GetLightPropsBuffer() const { return m_LightPropsBuffer.get(); }
		inline LightPosBuffer* GetLightPosBuffer() const { return m_LightPosBuffer.get(); }
		inline VertexShader* GetVertexShader() { return m_VertexShader; }
		inline PixelShader* GetPixelShader() { return m_PixelShader; }
		inline std::string GetMaterialInstanceName() const { return m_Name; }

		void BindMaterial();
		void BindShaderResources();
		void UnbindShaderResources();

	private:
		std::unordered_map<unsigned int, Texture2D*> m_Textures;
		std::unordered_map<unsigned int, std::unique_ptr<Sampler>> m_Samplers;
		std::unique_ptr<LightPosBuffer> m_LightPosBuffer;
		std::unique_ptr<LightPropsBuffer> m_LightPropsBuffer;
		std::unique_ptr<TransparencyBuffer> m_TransparencyBuffer;
		std::unique_ptr<LightDirectionBuffer> m_LightDirectionBuffer;
		std::unique_ptr<CameraBuffer> m_CameraBuffer;
		bool m_Illuminated;
		bool m_Transparent;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
		std::string m_Name;
	};
}

