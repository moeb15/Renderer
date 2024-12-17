#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/MaterialInstance.h"
#include <string>

namespace Yassin
{
	class Triangle : public Renderable
	{
	public:
		Triangle(std::string shader, DirectX::XMMATRIX world);
		virtual void Render(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const override;
	
	private:
		Topology m_Topology;
		std::shared_ptr<MaterialInstance> m_Material;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		VertexShader* m_VertexShader;
		PixelShader* m_PixelShader;
		std::shared_ptr<InputLayout> m_InputLayout;
		std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<Sampler> m_Sampler;
	};
}
