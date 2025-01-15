#include "Renderable/Utils/Mesh.h"
#include "Renderer/Resources/MaterialSystem.h"

namespace Yassin
{
	Mesh::Mesh(std::string material, DirectX::XMMATRIX world, std::vector<Vertex>& vertices,
		std::vector<unsigned long>& indices, std::vector<InstancePosition>* instancePositions)
	{
		m_InstancedDraw = instancePositions != nullptr;
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, instancePositions);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(world);

		m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
		// TODO properly fetch textures from model
		m_Material->SetTexture(TextureSlot::BaseTexture, "Stone");
		m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Clamp);
		m_Material->SetSampler(1, FilterType::Bilinear, AddressType::Wrap);
	}
}