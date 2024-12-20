#include "Renderer/Renderable.h"

namespace Yassin
{
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