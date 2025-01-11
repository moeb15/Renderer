#include "Renderable/Utils/OrthoWindow.h"
#include "Renderable/DataTypes.h"

namespace Yassin
{
	void OrthoWindow::Init(unsigned int windowWidth, unsigned int windowHeight)
	{
		float l, r, t, b;

		l = (windowWidth / 2.f) * -1.f;
		r = l + (float)windowWidth;
		t = (windowHeight / 2.f);
		b = t - (float)windowHeight;

		std::vector<Vertex> vData =
		{
			{{l,t,0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{r,b,0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
			{{l,b,0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
			{{l,t,0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{r,t,0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{r,b,0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		};

		std::vector<unsigned long> indices =
		{
			0, 1, 2,
			3, 4, 5
		};

		m_VertexBuffer = std::make_unique<VertexBuffer>(vData);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(DirectX::XMMatrixIdentity());
	}

	void OrthoWindow::Render(DirectX::XMMATRIX& viewProj) const
	{
		m_VertexBuffer->Bind(0);
		m_IndexBuffer->Bind();
		m_Topology.Bind();

		m_TransformBuffer->SetViewProjection(viewProj);
		m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
		m_TransformBuffer->SetTransformBuffer();
	}
}