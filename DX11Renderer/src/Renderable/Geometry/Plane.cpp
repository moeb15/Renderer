#include "Renderable/Geometry/Plane.h"
#include "Renderer/Resources/MaterialSystem.h"

namespace Yassin
{
	Plane::Plane(std::string material, DirectX::XMMATRIX world, int width, int height, float dimX, float dimY) :
		m_Width(width),
		m_Height(height),
		m_X(dimX),
		m_Y(dimY)
	{
		GeneratePlane();

		m_TransformBuffer = std::make_unique<TransformBuffer>();
		m_TransformBuffer->SetWorld(world);

		m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
		m_Material->SetTexture(TextureSlot::BaseTexture, "Metal");
		m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Clamp);
		m_Material->SetSampler(1, FilterType::Bilinear, AddressType::Wrap);

		m_BoundingBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(dimX + 0.25f, 1.25f, dimY + 0.25f));
		m_BoundX = dimX + 0.25f;
		m_BoundY = 1.25f;
		m_BoundZ = dimY + 0.25f;

		ConstructBoundingVolume(world);
	}

	void Plane::GeneratePlane()
	{
		unsigned int vertices = m_Width * m_Height;
		unsigned int faceCount = (m_Width - 1) * (m_Height - 1) * 2;

		float halfWidth = 0.5f * m_Width;
		float halfHeight = 0.5f * m_Height;

		float dx = m_X / (m_Width - 1);
		float dy = m_Y / (m_Height - 1);

		float du = 1.0f / (m_Width - 1);
		float dv = 1.0f / (m_Height - 1);

		std::vector<Vertex> vData;
		std::vector<unsigned long> indices;

		for (int i = 0; i < m_Height; i++)
		{
			float y = halfHeight - i * dy;
			for (int j = 0; j < m_Width; j++)
			{
				float x = -halfWidth + j * dx;

				Vertex vert;
				vert.position = DirectX::XMFLOAT3(x, 0, y);
				vert.normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
				vert.uv.x = j * du;
				vert.uv.y = i * dv;

				vData.push_back(vert);
			}
		}

		indices.resize(faceCount * 3);
		int k = 0;
		for (int i = 0; i < m_Height - 1; i++)
		{
			for (int j = 0; j < m_Width - 1; j++)
			{
				indices[k] = i * m_Width + j;
				indices[k + 1] = i * m_Width + j + 1;
				indices[k + 2] = (i + 1) * m_Width + j;
				indices[k + 3] = (i + 1) * m_Width + j;
				indices[k + 4] = i * m_Width + j + 1;
				indices[k + 5] = (i + 1) * m_Width + j + 1;

				k += 6;
			}
		}

		m_VertexBuffer = std::make_unique<VertexBuffer>(vData);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices);
	}
}