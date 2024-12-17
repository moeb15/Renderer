#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include <string>

namespace Yassin
{
	struct Vertex
	{
		Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 tex) : position(pos), uv(tex) {}
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
	};
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string material, DirectX::XMMATRIX world);
		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const override;
	
	private:
		Topology m_Topology;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
	};
}