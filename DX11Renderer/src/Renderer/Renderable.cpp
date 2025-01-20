#include "Renderer/Renderable.h"
#include "Renderer/Resources/MaterialInstance.h"
#include "Renderable/DataTypes.h"
#include "Renderable.h"
#include "Renderer/Resources/TextureLibrary.h"
#include "Renderer/Resources/MaterialSystem.h"

namespace Yassin
{
    void Renderable::Render(Camera& camera, DirectX::XMMATRIX& viewProj, bool bIgnoreMaterial, bool bRenderBoundingVolume, bool bDepthPrePass)
    {
        unsigned int curInstanceCount = (unsigned int) m_VertexBuffer->GetInstanceCount(); //FrustumCulling(camera, viewProj, bDepthPrePass);
        if (curInstanceCount == 0) return;

        if (bRenderBoundingVolume)
            RenderBoundingVolume(viewProj, curInstanceCount);
        
        m_VertexBuffer->Bind(0);
        m_IndexBuffer->Bind();
        m_Topology.Bind();

        m_TransformBuffer->SetViewProjection(viewProj);
        m_TransformBuffer->UpdateBuffer(m_TransformBuffer->GetMatrixBuffer());
        m_TransformBuffer->SetTransformBuffer();

        if (!bIgnoreMaterial)
            m_Material->BindMaterial();
        else
            m_Material->BindShaderResources();

        if (!m_InstancedDraw)
            RendererContext::GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
        else
            RendererContext::GetDeviceContext()->DrawIndexedInstanced(m_IndexBuffer->GetIndexCount(), curInstanceCount, 0, 0, 0);
    }

    unsigned int Renderable::FrustumCulling(Camera& camera, DirectX::XMMATRIX viewProj, bool bDepthPrePass)
    {
        size_t curInstanceCount = m_VertexBuffer->GetInstanceCount();
        if (curInstanceCount > 1)
        {
            for (int i = 0; i < m_InstancePositions.size(); i++)
            {
                DirectX::BoundingBox transformedBBox(DirectX::XMFLOAT3(m_InstancePositions[i].position), DirectX::XMFLOAT3(m_BoundX, m_BoundY, m_BoundZ));
                transformedBBox.Transform(transformedBBox, m_BoundingTransform->GetWorld());
                if (bDepthPrePass)
                {
                    DirectX::BoundingFrustum temp;
                    DirectX::BoundingFrustum::CreateFromMatrix(temp, viewProj);

                    if (!temp.Intersects(transformedBBox))
                    {
                        curInstanceCount -= 1;
                        if (curInstanceCount == 0) return 0;
                    }
                }
                else if (!camera.GetBoundingFrustum().Intersects(transformedBBox))
                {
                    curInstanceCount -= 1;
                    m_Culled += 1;
                    if (curInstanceCount == 0) return 0;
                }
            }
        }
        else
        {
            if (bDepthPrePass)
            {
                DirectX::BoundingFrustum temp;
                DirectX::BoundingFrustum::CreateFromMatrix(temp, viewProj);

                if (!temp.Intersects(GetBoundingBox()))
                {
                    curInstanceCount -= 1;
                    if (curInstanceCount == 0) return 0;
                }
            }
            else if (!camera.GetBoundingFrustum().Intersects(GetBoundingBox()))
            {
                curInstanceCount -= 1;
                m_Culled += 1;
                if (curInstanceCount == 0) return 0;
            }
        }

        return (unsigned int)curInstanceCount;
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

    void Renderable::BindShaderResources()
    {
        m_Material->BindShaderResources();
    }

    void Renderable::SetTexture(unsigned int slot, const std::string& texture)
    {
        m_Textures.emplace((TextureSlot) slot, texture);
        m_Material->SetTexture(slot, texture);
    }

    void Renderable::SetMaterial(std::string material)
    {
        Material* newMat = MaterialSystem::Get(material);
        if(newMat)
        {
            m_Material = std::make_unique<MaterialInstance>(newMat);
            for (const auto& kvPair : m_Textures) m_Material->SetTexture(kvPair.first, kvPair.second);
            m_Material->SetSampler(SamplerSlot::ClampSampler, FilterType::Bilinear, AddressType::Clamp);
            m_Material->SetSampler(SamplerSlot::WrapSampler, FilterType::Bilinear, AddressType::Wrap);
        }
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

        if(m_InstancePositions.size() > 0)
            m_BoundingVBuffer = std::make_unique<VertexBuffer>(vData, &m_InstancePositions);
        else
            m_BoundingVBuffer = std::make_unique<VertexBuffer>(vData);
        
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