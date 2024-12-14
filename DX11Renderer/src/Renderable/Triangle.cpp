#include "Renderable/Triangle.h"


namespace Yassin
{
    struct Vertex
    {
        Vertex(DirectX::XMFLOAT3 pos) : position(pos) {}
        DirectX::XMFLOAT3 position;
    };
    static std::vector<Vertex> vData = {
        Vertex({-1.f,-1.f,0.0f }),
        Vertex({0.0f,1.f,0.0f }),
        Vertex({1.f,-1.f,0.0f}),
    };

    static std::vector<unsigned long> iData = { 0,1,2 };

    Triangle::Triangle(std::string shader, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj) :
        m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
        m_VertexBuffer(vData),
        m_IndexBuffer(iData)
    {
        std::pair<VertexShader*, PixelShader*> shaderPair = ShaderLibrary::Get(shader);
        m_VertexShader = shaderPair.first;
        m_PixelShader = shaderPair.second;
        
        const D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        m_InputLayout = std::shared_ptr<InputLayout>(new InputLayout(m_VertexShader->GetBlob(), ied, std::size(ied)));
        m_TransformBuffer = std::shared_ptr<TransformBuffer>(new TransformBuffer());
        m_TransformBuffer->SetWorld(DirectX::XMMatrixTranspose(world));
        m_TransformBuffer->SetView(DirectX::XMMatrixTranspose(view));
        m_TransformBuffer->SetProjection(DirectX::XMMatrixTranspose(proj));
    }

    void Triangle::Render() const
    {
        m_VertexBuffer.Bind(0);
        m_IndexBuffer.Bind();
        m_Topology.Bind();
        
        m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
        m_TransformBuffer->SetTransformBuffer();

        m_InputLayout->Bind();

        m_VertexShader->Bind();
        m_PixelShader->Bind();

        RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer.GetIndexCount(), 0, 0);
    }
}