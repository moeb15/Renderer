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
#include "Renderer/Camera/Camera.h"

#include <DirectXCollision.h>

namespace Yassin
{
	class MaterialInstance;

	class Renderable
	{
	public:
		virtual void Render(Camera& camera, DirectX::XMMATRIX& viewProj, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj,
			bool bIgnoreMaterial = false, bool bRenderBoundingVolume = false, bool bDepthPrePass = false);
		virtual void RenderBoundingVolume(DirectX::XMMATRIX& viewProj, unsigned int instanceCount);
		virtual void UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const;
		virtual void UpdateCameraPosition(const CameraPositionType& cPos) const;
		virtual void UpdateLightDirection(const LightDirectionType& lDir) const;

		virtual void UpdateTransparency(float blendAmount = 1.0f);
		virtual void UpdateBoundingVolume() { m_BoundingBox.Transform(m_BoundingBox, m_BoundingTransform->GetWorld()); }

		virtual void BindShaderResources();
		virtual MaterialInstance* GetMaterialInstance(unsigned int meshIdx = 0) const { return m_Material.get(); }
		virtual VertexBuffer* GetVertexBuffer(unsigned int meshIdx = 0) const { return m_VertexBuffer.get(); }
		virtual IndexBuffer* GetIndexBuffer(unsigned int meshIdx = 0) const { return m_IndexBuffer.get(); }
		virtual TransformBuffer* GetTransformBuffer(unsigned int meshIdx = 0) { return m_TransformBuffer.get(); }


		inline const ObjectType& GetObjectType() const { return m_ObjectType; }
		inline const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }
		virtual const size_t GetCulledCount() const { return m_Culled; }
		virtual const size_t GetMeshCount() const { return 1; }
		inline DirectX::BoundingBox GetBoundingBox() { return this->m_BoundingBox; }

		virtual void SetTexture(unsigned int slot, const std::string& texture);
		virtual void SetMaterial(std::string material);
		void SetObjectVisiblity(ObjectVisibility visibility) { m_ObjectVisibility = visibility; }
		void SetObjectType(ObjectType objectType) { m_ObjectType = objectType; }
		virtual void ResetCulledCount() { m_Culled = 0; }
		virtual void SetShadowMap(ID3D11ShaderResourceView* srv);
		virtual void SetAmbientMap(ID3D11ShaderResourceView* srv);

		void ConstructBoundingVolume(DirectX::XMMATRIX& world);

		virtual void Translate(float x, float y, float z);
		virtual void Rotate(float yaw, float pitch, float roll);
		virtual void Scale(float x, float y, float z);

		unsigned int FrustumCulling(Camera& camera, DirectX::XMMATRIX viewProj, bool bDepthPrePass);

	protected:
		std::unique_ptr<TransformBuffer> m_TransformBuffer;
		std::unique_ptr<MaterialInstance> m_Material;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::vector<InstancePosition> m_InstancePositions;
		DirectX::BoundingBox m_BoundingBox;
		Topology m_Topology;
		ObjectType m_ObjectType = ObjectType::Geometry;
		ObjectVisibility m_ObjectVisibility = ObjectVisibility::Opaque;
		bool m_InstancedDraw = false;
		float m_BoundX;
		float m_BoundY;
		float m_BoundZ;

	private:
		std::unique_ptr<VertexBuffer> m_BoundingVBuffer;
		std::unique_ptr<IndexBuffer> m_BoundingIBuffer;
		std::unique_ptr<TransformBuffer> m_BoundingTransform;

		Sampler m_BoundingSampler;
		Topology m_BoundingTopology;
		size_t m_Culled;
		std::unordered_map<TextureSlot, std::string> m_Textures;
	};
}
