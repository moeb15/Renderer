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
	class Renderable
	{
	public:
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
	public:
		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const = 0;

		const ObjectType& GetObjectType() const { return m_ObjectType; }
		const ObjectVisibility GetObjectVisibility() const { return m_ObjectVisibility; }

	protected:
		ObjectType m_ObjectType = ObjectType::Geometry;
		ObjectVisibility m_ObjectVisibility = ObjectVisibility::Opaque;
	};
}
