#include "Renderer/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"

namespace Yassin
{
    void Renderable::Render(DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial) const
    {
        m_VertexBuffer->Bind(0);
        m_IndexBuffer->Bind();
        m_Topology.Bind();

        m_TransformBuffer->SetViewProjection(viewProj);
        m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
        m_TransformBuffer->SetTransformBuffer();

        if (!bIgnoreMaterial || !m_Material)
            m_Material->BindMaterial();

        if (!m_InstancedDraw)
            RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
        else
            RendererContext::GetDeviceContext()->DrawIndexedInstanced(m_IndexBuffer->GetIndexCount(),
                (unsigned int)m_VertexBuffer->GetInstanceCount(), 0, 0, 0);
    }

    void Renderable::UpdateLighting(const LightPositionBuffer& lPos, const LightPropertiesBuffer& lProps) const
    {
        m_Material->UpdateLightBuffers(lPos, lProps);
    }

    void Renderable::UpdateCameraPosition(const CameraPositionType& cPos) const
    {
        m_Material->UpdateCameraBuffer(cPos);
    }

    void Renderable::UpdateLightDirection(const LightDirectionType& lDir) const
    {
        m_Material->UpdateLightDirection(lDir);
    }

    void Renderable::UpdateTransparency(float blendAmount)
    {
        if (m_ObjectVisibility == ObjectVisibility::Transparent) m_Material->UpdateTransparencyBuffer({ blendAmount });
    }

    void Renderable::Translate(float x, float y, float z)
    {
        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(x, y, z);
        world = DirectX::XMMatrixMultiply(translation, world);
    }

    void Renderable::Rotate(float yaw, float pitch, float roll)
    {
        yaw = DirectX::XMConvertToRadians(yaw);
        pitch = DirectX::XMConvertToRadians(pitch);
        roll = DirectX::XMConvertToRadians(roll);

        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
        world = DirectX::XMMatrixMultiply(rotation, world);
    }

    void Renderable::Scale(float x, float y, float z)
    {
        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(x, y, z);
        world = DirectX::XMMatrixMultiply(scale, world);
    }
}