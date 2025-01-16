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
		virtual void UpdateCameraPosition(const CameraPositionType& cPos) const;
		virtual void UpdateLightDirection(const LightDirectionType& lDir) const;

		virtual void UpdateTransparency(float blendAmount = 1.0f);

		virtual MaterialInstance* GetMaterialInstance(unsigned int meshIdx = 0) const { return m_Material.get(); }
		virtual VertexBuffer* GetVertexBuffer(unsigned int meshIdx = 0) const { return m_VertexBuffer.get(); }
		virtual IndexBuffer* GetIndexBuffer(unsigned int meshIdx = 0) const { return m_IndexBuffer.get(); }
		virtual TransformBuffer* GetTransformBuffer(unsigned int meshIdx = 0) { return m_TransformBuffer.get(); }

		inline const ObjectType& GetObjectType() const { return m_ObjectType; }
		inline const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }
		
		void SetObjectVisiblity(ObjectVisibility visibility) { m_ObjectVisibility = visibility; }
		void SetObjectType(ObjectType objectType) { m_ObjectType = objectType; }

		virtual void Translate(float x, float y, float z);
		virtual void Rotate(float yaw, float pitch, float roll);
		virtual void Scale(float x, float y, float z);

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
