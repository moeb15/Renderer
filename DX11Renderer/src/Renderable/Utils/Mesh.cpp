#include "Renderable/Utils/Mesh.h"
#include "Renderer/Resources/MaterialSystem.h"

namespace Yassin
{
	Mesh::Mesh(std::string material, DirectX::XMMATRIX modelWorld, DirectX::XMMATRIX meshWorld, std::vector<Vertex>& vertices,
		std::vector<unsigned long>& indices, float boundsX, float boundsY, float boundsZ, const char* meshName,
		std::vector<InstancePosition>* instancePositions)
	{
		m_Name = meshName;

		if(instancePositions)
		{
			m_InstancedDraw = true;
			m_InstancePositions = *instancePositions;
		}

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, instancePositions);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(modelWorld);

		m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
		m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Clamp);
		m_Material->SetSampler(1, FilterType::Bilinear, AddressType::Wrap);

		m_BoundingBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(boundsX, boundsY, boundsZ));
		m_BoundX = boundsX;
		m_BoundY = boundsY;
		m_BoundZ = boundsZ;

		ConstructBoundingVolume(meshWorld);
	}
}