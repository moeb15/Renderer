#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string material, DirectX::XMMATRIX world);
		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const override;
	
	private:
		Topology m_Topology;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}