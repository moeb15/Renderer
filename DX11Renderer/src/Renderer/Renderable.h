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

namespace Yassin
{
	class MaterialInstance;
	
	enum class ObjectType
	{
		Geometry,
		Light,
	};

	enum class ObjectVisibility
	{
		Opaque,
		Transparent,
	};

	class Renderable
	{
	public:
		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const = 0;

		const ObjectType& GetObjectType() const { return m_ObjectType; }
		const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }

		void Translate(float x, float y, float z)
		{
			DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
			DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(x, y, z);
			world = DirectX::XMMatrixMultiply(translation, world);
			world = DirectX::XMMatrixTranspose(world);
		}

		void Rotate(float yaw, float pitch, float roll)
		{
			yaw = DirectX::XMConvertToRadians(yaw);
			pitch = DirectX::XMConvertToRadians(pitch);
			roll = DirectX::XMConvertToRadians(roll);

			DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
			DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
			world = DirectX::XMMatrixMultiply(rotation, world);
			world = DirectX::XMMatrixTranspose(world);
		}

		void Scale(float x, float y, float z)
		{
			DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
			DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(x, y, z);
			world = DirectX::XMMatrixMultiply(scale, world);
			world = DirectX::XMMatrixTranspose(world);
		}

	protected:
		std::unique_ptr<TransformBuffer> m_TransformBuffer;
		std::unique_ptr<MaterialInstance> m_Material;
		ObjectType m_ObjectType = ObjectType::Geometry;
		ObjectVisibility m_ObjectVisibility = ObjectVisibility::Opaque;
	};
}
