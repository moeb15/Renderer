#pragma once
#include "Renderer/RendererContext.h"
#include "Renderer/Renderable.h"

namespace Yassin
{
	class OrthoWindow
	{
	public:
		void Init(unsigned int windowWidth, unsigned int windowHeight);
		void Render(DirectX::XMMATRIX& viewProj, DirectX::XMMATRIX& view = DirectX::XMMatrixIdentity(), DirectX::XMMATRIX& proj = DirectX::XMMatrixIdentity()) const;

	private:
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<TransformBuffer> m_TransformBuffer;
		Topology m_Topology = Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	};
}

