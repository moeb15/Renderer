#pragma once
#include "Renderer/Renderable.h"
#include <string>

namespace Yassin
{
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string shader, 
			DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
		void Render() const override;
	
	private:
		Topology m_Topology;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
		std::shared_ptr<InputLayout> m_InputLayout;
		std::shared_ptr<TransformBuffer> m_TransformBuffer;
	};
}
