#include "Renderer/Camera/Camera.h"


namespace Yassin
{
	Camera::Camera(float FOV, float aspectRatio) :
		m_Position(0.0f, 0.0f, 0.0f),
		m_Yaw(0.0f),
		m_Pitch(0.0f),
		m_Roll(0.0f),
		m_Forward(0.0f, 0.0f, 1.0f),
		m_Right(1.0f, 0.0f, 0.0f),
		m_Up(0.0f, 1.0f, 0.0f)
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(FOV),
			aspectRatio,
			0.1f,
			1000.f);

		DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, proj);
		
		DirectX::XMFLOAT3 up, pos, lookAt;
		DirectX::XMVECTOR upVec, posVec, lookAtVec;
		float yaw, pitch, roll;
		DirectX::XMMATRIX rotMat;

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		upVec = DirectX::XMLoadFloat3(&up);

		pos.x = m_Position.x;
		pos.y = m_Position.y;
		pos.z = m_Position.z;

		posVec = DirectX::XMLoadFloat3(&pos);

		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		lookAtVec = DirectX::XMLoadFloat3(&lookAt);

		yaw = DirectX::XMConvertToRadians(m_Yaw);
		pitch = DirectX::XMConvertToRadians(m_Pitch);
		roll = DirectX::XMConvertToRadians(m_Roll);

		rotMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		lookAtVec = DirectX::XMVector3TransformCoord(lookAtVec, rotMat);
		upVec = DirectX::XMVector3TransformCoord(upVec, rotMat);
		lookAtVec = DirectX::XMVectorAdd(posVec, lookAtVec);

		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec);
		DirectX::XMStoreFloat4x4(&m_DefaultView, view);

		DirectX::BoundingFrustum::CreateFromMatrix(m_ViewFrustum, DirectX::XMMatrixMultiply(view, proj));

		UpdateView();
	}

	void Camera::UpdateView()
	{
		DirectX::XMFLOAT3 up, pos, lookAt;
		DirectX::XMVECTOR upVec, posVec, lookAtVec;
		float yaw, pitch, roll;
		DirectX::XMMATRIX rotMat;

		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		upVec = DirectX::XMLoadFloat3(&up);

		pos.x = m_Position.x;
		pos.y = m_Position.y;
		pos.z = m_Position.z;

		posVec = DirectX::XMLoadFloat3(&pos);

		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 1.0f;

		lookAtVec = DirectX::XMLoadFloat3(&lookAt);

		yaw = DirectX::XMConvertToRadians(m_Yaw);
		pitch = DirectX::XMConvertToRadians(m_Pitch);
		roll = DirectX::XMConvertToRadians(m_Roll);

		rotMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		lookAtVec = DirectX::XMVector3TransformCoord(lookAtVec, rotMat);
		upVec = DirectX::XMVector3TransformCoord(upVec, rotMat);

		DirectX::XMStoreFloat3(&m_Up, upVec);
		DirectX::XMStoreFloat3(&m_Forward, lookAtVec);

		DirectX::XMVECTOR right = DirectX::XMVector3Cross(upVec, lookAtVec);
		DirectX::XMStoreFloat3(&m_Right, right);

		lookAtVec = DirectX::XMVectorAdd(posVec, lookAtVec);
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec);
		DirectX::XMStoreFloat4x4(&m_ViewMatrix, view);

		DirectX::BoundingFrustum temp(DirectX::XMLoadFloat4x4(&m_ProjectionMatrix));
		temp.Transform(m_ViewFrustum, DirectX::XMMatrixInverse(nullptr, view));

	}
}