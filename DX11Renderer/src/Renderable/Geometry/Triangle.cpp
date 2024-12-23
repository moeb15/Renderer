#include "Renderable/Geometry/Triangle.h"
#include "Renderer/MaterialSystem.h"
#include "Triangle.h"


namespace Yassin
{
    static std::vector<Vertex> vData = {
        Vertex({-1.f,-1.f,0.0f }, {0.0f, 0.0f}, {}),
        Vertex({0.0f,1.f,0.0f }, {0.5f, 1.0f}, {}),
        Vertex({1.f,-1.f,0.0f}, {1.0f, 0.0f}, {}),
    };

    static std::vector<unsigned long> iData = { 0,1,2 };

    Triangle::Triangle(std::string material, DirectX::XMMATRIX world) :
        m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    { 
        m_VertexBuffer = std::make_unique<VertexBuffer>(vData);
        m_IndexBuffer = std::make_unique<IndexBuffer>(iData);

        m_Material = std::make_unique<MaterialInstance>(MaterialSystem::Get(material));
        m_Material->SetTexture(0, "");
        m_Material->SetSampler(0, FilterType::Bilinear, AddressType::Wrap);

        m_TransformBuffer = std::make_unique<TransformBuffer>();
        m_TransformBuffer->SetWorld(world);
    }

    void Triangle::Render(DirectX::XMMATRIX& viewProj) const
    {
        m_VertexBuffer->Bind(0);
        m_IndexBuffer->Bind();
        m_Topology.Bind();
        
        m_TransformBuffer->SetViewProjection(viewProj);
        m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
        m_TransformBuffer->SetTransformBuffer();

        m_Material->BindMaterial();

        RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
    }

    void Yassin::Triangle::UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const
    {
        m_Material->UpdateLightBuffers(lPos, lProps);
    }
}