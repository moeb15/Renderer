#pragma once
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexShader.h"
#include "Renderer/PixelShader.h"
#include "Renderer/InputLayout.h"
#include "Renderer/Topology.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/Texture.h"
#include "Renderer/Sampler.h"

#include "Renderer/ShaderLibrary.h"
#include "Renderer/DataTypeEnums.h"

namespace Yassin
{
	class MaterialInstance;

	class Renderable
	{
	public:
		virtual void Render(DirectX::XMMATRIX& viewProj) const = 0;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const = 0;
		
		virtual void UpdateShadowMap(ID3D11ShaderResourceView* srv) const 
		{
			RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::DepthMapTexture, 1, &srv);
		}
		
		virtual void UnbindSRV() const 
		{
			ID3D11ShaderResourceView* nullSRV = { nullptr };
			RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::BaseTexture, 1, &nullSRV);
			RendererContext::GetDeviceContext()->PSSetShaderResources(TextureSlot::DepthMapTexture, 1, &nullSRV);
		}

		inline MaterialInstance* GetMaterialInstance() const { return m_Material.get(); }
		inline VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }
		inline IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer.get(); }
		inline TransformBuffer* GetTransformBuffer() { return m_TransformBuffer.get(); }

		inline const ObjectType& GetObjectType() const { return m_ObjectType; }
		inline const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }

		void Translate(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);
		void Scale(float x, float y, float z);

	protected:
		std::unique_ptr<TransformBuffer> m_TransformBuffer;
		std::unique_ptr<MaterialInstance> m_Material;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		ObjectType m_ObjectType = ObjectType::Geometry;
		ObjectVisibility m_ObjectVisibility = ObjectVisibility::Opaque;
	};
}
