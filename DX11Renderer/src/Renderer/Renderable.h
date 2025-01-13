#pragma once
#include "Renderer/Primitives/VertexBuffer.h"
#include "Renderer/Primitives/IndexBuffer.h"
#include "Renderer/Primitives/VertexShader.h"
#include "Renderer/Primitives/PixelShader.h"
#include "Renderer/Primitives/InputLayout.h"
#include "Renderer/Primitives/Topology.h"
#include "Renderer/Primitives/ConstantBuffer.h"
#include "Renderer/Primitives/Texture.h"
#include "Renderer/Primitives/Sampler.h"

#include "Renderer/Resources/ShaderLibrary.h"
#include "Renderer/DataTypeEnums.h"

namespace Yassin
{
	class MaterialInstance;

	class Renderable
	{
	public:
		virtual void Render(DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial = false) const;
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const;
		
		virtual void UpdateTransparency(float blendAmount = 1.0f);

		inline MaterialInstance* GetMaterialInstance() const { return m_Material.get(); }
		inline VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }
		inline IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer.get(); }
		inline TransformBuffer* GetTransformBuffer() { return m_TransformBuffer.get(); }

		inline const ObjectType& GetObjectType() const { return m_ObjectType; }
		inline const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }
		
		void SetObjectVisiblity(ObjectVisibility visibility) { m_ObjectVisibility = visibility; }
		void SetObjectType(ObjectType objectType) { m_ObjectType = objectType; }

		void Translate(float x, float y, float z);
		void Rotate(float yaw, float pitch, float roll);
		void Scale(float x, float y, float z);

	protected:
		std::unique_ptr<TransformBuffer> m_TransformBuffer;
		std::unique_ptr<MaterialInstance> m_Material;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		Topology m_Topology;
		ObjectType m_ObjectType = ObjectType::Geometry;
		ObjectVisibility m_ObjectVisibility = ObjectVisibility::Opaque;
		bool m_InstancedDraw = false;
	};
}
