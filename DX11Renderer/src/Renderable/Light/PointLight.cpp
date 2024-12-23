#include "Renderable/Light/PointLight.h"


namespace Yassin
{
	void PointLight::UpdateView()
	{
		DirectX::XMFLOAT3 up(0.0f, 1.0f, 0.0f);
		DirectX::XMVECTOR upVec, lookAtVec, posVec;

		upVec = DirectX::XMLoadFloat3(&up);
		lookAtVec = DirectX::XMLoadFloat3(&m_LookAt);
		posVec = DirectX::XMLoadFloat3(&m_Position);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec);
		DirectX::XMStoreFloat4x4(&m_ViewMatrix, view);
	}
}
