#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include <string>

namespace Yassin
{
	class Plane : public Renderable
	{
	public:
		Plane(std::string material, DirectX::XMMATRIX world, int width, int height, float dimX, float dimY);

		void GeneratePlane();

		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const override;

	private:
		Topology m_Topology;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		int m_Width;
		int m_Height;
		float m_X;
		float m_Y;
	};
}
