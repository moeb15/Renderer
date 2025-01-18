#include "Renderer/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include "Renderable.h"
#include "Renderer/Resources/TextureLibrary.h"

namespace Yassin
{
    void Renderable::Render(Camera& camera, DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial, bool bRenderBoundingVolume)
    {
        size_t instanceCount = m_VertexBuffer->GetInstanceCount();
        DirectX::BoundingBox transformedBBox = m_BoundingBox;
        DirectX::XMMATRIX view;
        camera.GetViewMatrix(view);
        transformedBBox.Transform(transformedBBox, view);
        if (!camera.GetBoundingFrustum().Intersects(m_BoundingBox))
        {
            instanceCount -= 1;
            if (instanceCount == 0) return;
        }

        if (bRenderBoundingVolume)
            RenderBoundingVolume(viewProj, (unsigned int)instanceCount);
        
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
                (unsigned int)instanceCount, 0, 0, 0);
    }

    void Renderable::RenderBoundingVolume(DirectX::XMMATRIX& viewProj, unsigned int instanceCount)
    {
        std::pair<VertexShader*, PixelShader*> bVolumeShaders = ShaderLibrary::Get("Bounding Volume Shader");

        m_BoundingVBuffer->Bind(0);
        m_BoundingIBuffer->Bind();
        m_BoundingTopology.Bind();

        m_BoundingTransform->SetViewProjection(viewProj);
        m_BoundingTransform->UpdateBuffer(m_BoundingTransform->GetMatrixBuffer());
        m_BoundingTransform->SetTransformBuffer();

        Texture2D* tex = TextureLibrary::GetTexture2D("BoundingVolume");
        bVolumeShaders.second->SetTexture(TextureSlot::BaseTexture, tex->GetTexture());
        m_BoundingSampler.Bind(SamplerSlot::WrapSampler);
        bVolumeShaders.first->Bind();
        bVolumeShaders.second->Bind();

        if (!m_InstancedDraw)
            RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
        else
            RendererContext::GetDeviceContext()->DrawIndexedInstanced(m_BoundingIBuffer->GetIndexCount(),
                (unsigned int)instanceCount, 0, 0, 0);
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

    void Renderable::ConstructBoundingVolume(DirectX::XMMATRIX& world)
    {
        std::vector<Vertex> vData =
        {
            // front
            { { -m_BoundX, -m_BoundY, -m_BoundZ }, { 0.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
            { { -m_BoundX, m_BoundY, -m_BoundZ }, { 0.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
            { { m_BoundX, m_BoundY, -m_BoundZ }, { 1.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
            { { m_BoundX, -m_BoundY, -m_BoundZ }, { 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },

            // back
            { { -m_BoundX, -m_BoundY,  m_BoundZ }, { 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
            { {  m_BoundX, -m_BoundY,  m_BoundZ }, { 0.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
            { {  m_BoundX,  m_BoundY,  m_BoundZ }, { 0.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
            { { -m_BoundX,  m_BoundY,  m_BoundZ }, { 1.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },

            // left
           { { -m_BoundX, -m_BoundY,  m_BoundZ }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
           { { -m_BoundX,  m_BoundY,  m_BoundZ }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
           { { -m_BoundX,  m_BoundY, -m_BoundZ }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
           { { -m_BoundX, -m_BoundY, -m_BoundZ }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },

           // right
           { {  m_BoundX, -m_BoundY, -m_BoundZ }, { 0.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
           { {  m_BoundX,  m_BoundY, -m_BoundZ }, { 0.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
           { {  m_BoundX,  m_BoundY,  m_BoundZ }, { 1.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
           { {  m_BoundX, -m_BoundY,  m_BoundZ }, { 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },

           // top
           { { -m_BoundX,  m_BoundY, -m_BoundZ }, { 0.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
           { { -m_BoundX,  m_BoundY,  m_BoundZ }, { 0.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
           { {  m_BoundX,  m_BoundY,  m_BoundZ }, { 1.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
           { {  m_BoundX,  m_BoundY, -m_BoundZ }, { 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },

           // bottom
           { { -m_BoundX, -m_BoundY, -m_BoundZ }, { 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
           { {  m_BoundX, -m_BoundY, -m_BoundZ }, { 0.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
           { {  m_BoundX, -m_BoundY,  m_BoundZ }, { 0.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
           { { -m_BoundX, -m_BoundY,  m_BoundZ }, { 1.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
        };

        std::vector<unsigned long> indices =
        {
            // Front face
            0, 1, 2, 0, 2, 3,

            // Back face
            4, 5, 6, 4, 6, 7,

            // Left face
            8, 9, 10, 8, 10, 11,

            // Right face
            12, 13, 14, 12, 14, 15,

            // Top face
            16, 17, 18, 16, 18, 19,

            // Bottom face
            20, 21, 22, 20, 22, 23
        };

        m_BoundingVBuffer = std::make_unique<VertexBuffer>(vData, &m_InstancePositions);
        m_BoundingIBuffer = std::make_unique<IndexBuffer>(indices);
        m_BoundingSampler.Init(FilterType::Bilinear, AddressType::Wrap);

        m_BoundingTransform = std::make_unique<TransformBuffer>();
        m_BoundingTransform->SetWorld(world);
        UpdateBoundingVolume();
    }

    void Renderable::Translate(float x, float y, float z)
    {
        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(x, y, z);
        world = DirectX::XMMatrixMultiply(translation, world);
        m_BoundingTransform->SetWorld(world);
        UpdateBoundingVolume();
    }

    void Renderable::Rotate(float yaw, float pitch, float roll)
    {
        yaw = DirectX::XMConvertToRadians(yaw);
        pitch = DirectX::XMConvertToRadians(pitch);
        roll = DirectX::XMConvertToRadians(roll);

        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
        world = DirectX::XMMatrixMultiply(rotation, world);
        m_BoundingTransform->SetWorld(world);
        UpdateBoundingVolume();
    }

    void Renderable::Scale(float x, float y, float z)
    {
        DirectX::XMMATRIX& world = m_TransformBuffer->GetWorld();
        DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(x, y, z);
        world = DirectX::XMMatrixMultiply(scale, world);
        m_BoundingTransform->SetWorld(world);
        UpdateBoundingVolume();
    }
}