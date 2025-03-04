#include "Renderable/Geometry/Triangle.h"
#include "Renderer/Resources/MaterialSystem.h"


namespace Yassin
{
    static std::vector<Vertex> vData = {
        Vertex({-1.f,-1.f,0.0f }, {0.0f, 0.0f}, {}),
        Vertex({0.0f,1.f,0.0f }, {0.5f, 1.0f}, {}),
        Vertex({1.f,-1.f,0.0f}, {1.0f, 0.0f}, {}),
    };

    static std::vector<unsigned long> iData = { 0,1,2 };

    Triangle::Triangle(std::string material, DirectX::XMMATRIX world)
    { 
        m_VertexBuffer = std::make_unique<VertexBuffer>(vData);
        m_IndexBuffer = std::make_unique<IndexBuffer>(iData);

        m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
        m_Material->SetTexture(0, "");
        m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Wrap);

        m_TransformBuffer = std::make_unique<TransformBuffer>();
        m_TransformBuffer->SetWorld(world);
    }
}